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
		HierarchicalArray2D(int xsize, int ysize, int patchMagnitude=5);
		HierarchicalArray2D(const HierarchicalArray2D& hg);
		HierarchicalArray2D& operator=(const HierarchicalArray2D& hg);
		virtual ~HierarchicalArray2D(){}
		void resize(int ixmin, int iymin, int ixmax, int iymax);
		inline int getPatchSize() const {return m_patchMagnitude;}
		inline int getPatchMagnitude() const {return m_patchMagnitude;}
		
		inline AccessibilityState cellState(int x, int y) const ;
		inline IntPoint patchIndexes(int x, int y) const;
		
		inline const Cell& cell(const IntPoint& p) const { return cell(p.x,p.y); }
		inline Cell& cell(const IntPoint& p) { return cell(p.x,p.y); }
		inline AccessibilityState cellState(const IntPoint& p) const { return cellState(p.x,p.y); }
		inline IntPoint patchIndexes(const IntPoint& p) const { return patchIndexes(p.x,p.y);}
		
		inline void setActiveArea(const PointSet&, bool patchCoords=false);
		const PointSet& getActiveArea() const {return m_activeArea; }
		inline void allocActiveArea();
	protected:
		inline bool isAllocated(const IntPoint& p) const;
		inline bool isAllocated(int ii) const;
		inline int patchIndex(int x, int y) const;
		inline const Cell& cell(int x, int y) const;
		inline Cell& cell(int x, int y);

		virtual Array2D<Cell> * createPatch() const;
		PointSet m_activeArea;
		int m_patchMagnitude;
		int m_patchSize;
};

template <class Cell>
HierarchicalArray2D<Cell>::HierarchicalArray2D(int xsize, int ysize, int patchMagnitude) 
  :Array2D<autoptr< Array2D<Cell> > >::Array2D((xsize>>patchMagnitude), (ysize>>patchMagnitude)){
	m_patchMagnitude=patchMagnitude;
	m_patchSize=1<<m_patchMagnitude;
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
	this->m_patchSize=hg.m_patchSize;
}

template <class Cell>
void HierarchicalArray2D<Cell>::resize(int xmin, int ymin, int xmax, int ymax){
	int xsize=xmax-xmin;
	int ysize=ymax-ymin;
	autoptr< Array2D<Cell> > * newcells=new autoptr< Array2D<Cell> >[xsize*ysize];
	for (int i=0; i<(xsize*ysize); i++) {
		newcells[i]=autoptr< Array2D<Cell> >(0);
	}
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
	m_patchSize=hg.m_patchSize;
	return *this;
}


template <class Cell>
void HierarchicalArray2D<Cell>::setActiveArea(const typename HierarchicalArray2D<Cell>::PointSet& aa, bool patchCoords){
	m_activeArea.clear();
	for (PointSet::const_iterator it= aa.begin(); it!=aa.end(); ++it) {
		IntPoint p;
		if (patchCoords)
			p=*it;
		else
			p=patchIndexes(*it);
		m_activeArea.insert(p);
	}
}

template <class Cell>
Array2D<Cell>* HierarchicalArray2D<Cell>::createPatch() const{
	return new Array2D<Cell>(1<<m_patchMagnitude, 1<<m_patchMagnitude);
}


template <class Cell>
AccessibilityState  HierarchicalArray2D<Cell>::cellState(int x, int y) const {
	IntPoint c=patchIndexes(x,y);
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
		const autoptr< Array2D<Cell> >& ptr=this->m_cells[ii];
		//Array2D<Cell>* patch=0;
		if (!ptr){
			//patch=createPatch();
		  this->m_cells[ii]=autoptr< Array2D<Cell> >(createPatch());
      /*
		} else{	
			patch=new Array2D<Cell>(*ptr);
      */
		}
		//this->m_cells[ii]=autoptr< Array2D<Cell> >(patch);
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
IntPoint HierarchicalArray2D<Cell>::patchIndexes(int x, int y) const{
	if (x>=0 && y>=0)
		return IntPoint(x>>m_patchMagnitude, y>>m_patchMagnitude);
	return IntPoint(-1, -1);
}

template <class Cell>
int HierarchicalArray2D<Cell>::patchIndex(int x, int y) const{
	if (x>=0 && y>=0)
		return this->cellIndex(x>>m_patchMagnitude, y>>m_patchMagnitude);
	return 0;
}


template <class Cell>
Cell& HierarchicalArray2D<Cell>::cell(int x, int y){
	IntPoint c=patchIndexes(x,y);
	assert(this->isInside(c.x, c.y));
	const int ii = this->cellIndex(c.x, c.y);
	if (!this->m_cells[ii]){
		Array2D<Cell>* patch=createPatch();
		this->m_cells[ii]=autoptr< Array2D<Cell> >(patch);
		//cerr << "!!! FATAL !!!" << endl;
	}
	autoptr< Array2D<Cell> >& ptr=this->m_cells[ii];
	return (*ptr).cell(x-(c.x<<m_patchMagnitude), y-(c.y<<m_patchMagnitude));
}

template <class Cell>
const Cell& HierarchicalArray2D<Cell>::cell(int x, int y) const{
	const int ii = patchIndex(x,y);
	assert(isAllocated(ii));
	const autoptr< Array2D<Cell> >& ptr=this->m_cells[ii];
	//return (*ptr).cell(x-(c.x<<m_patchMagnitude), y-(c.y<<m_patchMagnitude));
	return (*ptr).cell(x % m_patchSize, y % m_patchSize);
}

};

#endif

