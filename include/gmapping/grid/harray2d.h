#ifndef HARRAY2D_H
#define HARRAY2D_H
#include <set>
#include <gmapping/utils/point.h>
#include <gmapping/utils/autoptr.h>
#include "array2d.h"

namespace GMapping {

template <class Cell>
class HierarchicalArray2D: public Array2D<autoptr< Array2D<Cell> > >{
	public:
		typedef std::set< point<int>, pointcomparator<int> > PointSet;
		HierarchicalArray2D(int xsize, int ysize, int patchMagnitude=6);
		HierarchicalArray2D(const HierarchicalArray2D& hg);
		HierarchicalArray2D& operator=(const HierarchicalArray2D& hg);
		virtual ~HierarchicalArray2D(){}
		void resize(int ixmin, int iymin, int ixmax, int iymax);
		inline int getPatchSize() const {return m_patchMagnitude;}
		inline int getPatchMagnitude() const {return m_patchMagnitude;}
		
		
		inline const Cell& cell(const IntPoint& p) const;
		inline Cell& cell(const IntPoint& p);
		inline AccessibilityState cellState(const IntPoint& p) const;
		inline IntPoint patchIndexes(const IntPoint& p) const;
		
		inline void setActiveArea(const PointSet&, bool patchCoords=false);
		const PointSet& getActiveArea() const {return m_activeArea; }
		inline void allocActiveArea();
	protected:
		inline bool isAllocated(const IntPoint& p) const;
		inline bool isAllocated(int ii) const;

		PointSet m_activeArea;
		int m_patchMagnitude;
};

template <class Cell>
HierarchicalArray2D<Cell>::HierarchicalArray2D(int xsize, int ysize, int patchMagnitude) 
  :Array2D<autoptr< Array2D<Cell> > >::Array2D((xsize>>patchMagnitude), (ysize>>patchMagnitude)){
	m_patchMagnitude=patchMagnitude;
}

template <class Cell>
HierarchicalArray2D<Cell>::HierarchicalArray2D(const HierarchicalArray2D& hg)
  :Array2D<autoptr< Array2D<Cell> > >::Array2D((hg.m_xsize>>hg.m_patchMagnitude), (hg.m_ysize>>hg.m_patchMagnitude))  // added by cyrill: if you have a resize error, check this again
{
	this->m_xsize=hg.m_xsize;
	this->m_ysize=hg.m_ysize;
	this->m_cells=new autoptr< Array2D<Cell> >[this->m_xsize * this->m_ysize];
	for (int i=0; i<(this->m_xsize*this->m_ysize); i++) {
		this->m_cells[i] = hg.m_cells[i];
	}
	this->m_patchMagnitude=hg.m_patchMagnitude;
}

template <class Cell>
void HierarchicalArray2D<Cell>::resize(int xmin, int ymin, int xmax, int ymax){
	int xsize=xmax-xmin;
	int ysize=ymax-ymin;
	autoptr< Array2D<Cell> > * newcells=new autoptr< Array2D<Cell> >[xsize*ysize];
	int dx= xmin < 0 ? 0 : xmin;
	int dy= ymin < 0 ? 0 : ymin;
	int Dx=xmax<this->m_xsize?xmax:this->m_xsize;
	int Dy=ymax<this->m_ysize?ymax:this->m_ysize;
	for (int x=dx; x<Dx; x++){
		for (int y=dy; y<Dy; y++){
			const int ii = (x-xmin) + ((y-ymin)*xsize);
			newcells[ii]=this->m_cells[this->cellIndex(x, y)];
		}
	}
	delete [] this->m_cells;
	this->m_cells=newcells;
	this->m_xsize=xsize;
	this->m_ysize=ysize; 
}

template <class Cell>
HierarchicalArray2D<Cell>& HierarchicalArray2D<Cell>::operator=(const HierarchicalArray2D& hg){
//	Array2D<autoptr< Array2D<Cell> > >::operator=(hg);
	if (this->m_xsize!=hg.m_xsize || this->m_ysize!=hg.m_ysize){
		delete [] this->m_cells;
		this->m_xsize=hg.m_xsize;
		this->m_ysize=hg.m_ysize;
		this->m_cells=new autoptr< Array2D<Cell> >[this->m_xsize * this->m_ysize];
	}
	for (int i=0; i<(this->m_xsize * this->m_ysize); i++)
		this->m_cells[i]=hg.m_cells[i];
	
	m_activeArea.clear();
	m_patchMagnitude=hg.m_patchMagnitude;
	return *this;
}


template <class Cell>
void HierarchicalArray2D<Cell>::setActiveArea(const typename HierarchicalArray2D<Cell>::PointSet& aa, bool patchCoords){
	m_activeArea.clear();
  if(patchCoords) {
    m_activeArea.insert(aa.begin(), aa.end());
  } else {
	  for (PointSet::const_iterator it= aa.begin(); it!=aa.end(); ++it) {
      m_activeArea.insert(patchIndexes(*it));
    }
	}
}

template <class Cell>
AccessibilityState  HierarchicalArray2D<Cell>::cellState(const IntPoint &p) const {
	IntPoint c=patchIndexes(p);
	if (this->isInside(c)) {
		if(isAllocated(c))
			return (AccessibilityState)((int)Inside|(int)Allocated);
		else
			return Inside;
	}
	return Outside;
}

template <class Cell>
void HierarchicalArray2D<Cell>::allocActiveArea(){
	for (PointSet::const_iterator it= m_activeArea.begin(); it!=m_activeArea.end(); ++it){
		const int ii = this->cellIndex(it->x, it->y);
	  autoptr< Array2D<Cell> >& ptr=this->m_cells[ii];
    if(ptr == 0) {
      this->m_cells[ii].create();
      (*this->m_cells[ii]).resize(0, 0, 1<<m_patchMagnitude, 1<<m_patchMagnitude);
    }
	}
}

template <class Cell>
bool HierarchicalArray2D<Cell>::isAllocated(const IntPoint& c) const{
  return isAllocated(this->cellIndex(c.x, c.y));
}

template <class Cell>
bool HierarchicalArray2D<Cell>::isAllocated(int ii) const{
	autoptr< Array2D<Cell> >& ptr=this->m_cells[ii];
	return (ptr != 0);
}


template <class Cell>
IntPoint HierarchicalArray2D<Cell>::patchIndexes(const IntPoint &p) const {
	if (p.x>=0 && p.y>=0)
		return IntPoint(p.x>>m_patchMagnitude, p.y>>m_patchMagnitude);
	return IntPoint(-1, -1);
}


template <class Cell>
Cell& HierarchicalArray2D<Cell>::cell(const IntPoint &p) {
	IntPoint c=patchIndexes(p);
	assert(this->isInside(c.x, c.y));
	const int ii = this->cellIndex(c.x, c.y);
  if(this->m_cells[ii] == 0) {
    this->m_cells[ii].create();
    (*this->m_cells[ii]).resize(0, 0, 1<<m_patchMagnitude, 1<<m_patchMagnitude);
  }
	autoptr< Array2D<Cell> >& ptr=this->m_cells[ii];
	return (*ptr).cell(p.x-(c.x<<m_patchMagnitude), p.y-(c.y<<m_patchMagnitude));
}

template <class Cell>
const Cell& HierarchicalArray2D<Cell>::cell(const IntPoint &p) const{
	IntPoint c=patchIndexes(p);
	const int ii = this->cellIndex(c.x,c.y);
	assert(isAllocated(ii));
	const autoptr< Array2D<Cell> >& ptr=this->m_cells[ii];
	return (*ptr).cell(p.x-(c.x<<m_patchMagnitude), p.y-(c.y<<m_patchMagnitude));
}

};

#endif

