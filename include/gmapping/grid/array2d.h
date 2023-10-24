#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <memory>
#include <assert.h>
#include <gmapping/utils/point.h>
#include "gmapping/grid/accessstate.h"

#include <iostream>

namespace GMapping {

template<class Cell, const bool debug=false> class Array2D{
	public:
    using CellArray = std::shared_ptr<Cell[]>;
    using CellArray2d = std::shared_ptr<CellArray[]>;
		Array2D(int xsize=0, int ysize=0);
		Array2D& operator=(const Array2D &);
		Array2D(const Array2D<Cell,debug> &);
		~Array2D();
		void clear();
		void resize(int xmin, int ymin, int xmax, int ymax);
		
		
		inline bool isInside(int x, int y) const;
		inline const Cell& cell(int x, int y) const;
		inline Cell& cell(int x, int y);
		inline AccessibilityState cellState(int x, int y) const { return (AccessibilityState) (isInside(x,y)?(Inside|Allocated):Outside);}
		
		inline bool isInside(const IntPoint& p) const { return isInside(p.x, p.y);}
		inline const Cell& cell(const IntPoint& p) const {return cell(p.x,p.y);}
		inline Cell& cell(const IntPoint& p) {return cell(p.x,p.y);}
		inline AccessibilityState cellState(const IntPoint& p) const { return cellState(p.x, p.y);}
		
		inline int getPatchSize() const{return 0;}
		inline int getPatchMagnitude() const{return 0;}
		inline int getXSize() const {return m_xsize;}
		inline int getYSize() const {return m_ysize;}
		inline CellArray2d cells() {return m_cells;}
    CellArray2d m_cells;
	protected:
		int m_xsize = 0, m_ysize = 0;
};


template <class Cell, const bool debug>
Array2D<Cell,debug>::Array2D(int xsize, int ysize){
//	assert(xsize>0);
//	assert(ysize>0);
  // delete memory
  if (m_cells)
  {
    m_cells.reset();
  }
  m_xsize=xsize;
  m_ysize=ysize;
	if (m_xsize>0 && m_ysize>0)
  {
    m_cells=CellArray2d(new CellArray[m_xsize], [](auto ptr){ delete[] ptr; });
		for (int i=0; i<m_xsize; i++)
    {
      m_cells[i]=CellArray(new Cell[m_ysize], [](auto ptr){ delete[] ptr; });
    }
	}
	else
  {
    std::cerr << __func__ << "Invalid size: " << "m_xsize= " << m_xsize << " m_ysize= " << m_ysize << " - resetting " << std::endl;
		m_xsize=m_ysize=0;
    m_cells.reset();
	}
	if (debug)
  {
		std::cerr << __func__ << std::endl;
		std::cerr << "m_xsize= " << m_xsize<< std::endl;
		std::cerr << "m_ysize= " << m_ysize<< std::endl;
	}
}

template <class Cell, const bool debug>
Array2D<Cell,debug> & Array2D<Cell,debug>::operator=(const Array2D<Cell,debug> & g){
	if (debug || m_xsize!=g.m_xsize || m_ysize!=g.m_ysize)
  {
    // delete memory
    if (m_cells)
    {
      m_cells.reset();
    }
		m_xsize=g.m_xsize;
		m_ysize=g.m_ysize;
    if (m_xsize > 0 && m_ysize > 0)
    {
      m_cells = CellArray2d(new CellArray[m_xsize], [](auto ptr) { delete[] ptr; });
      for (int i = 0; i < m_xsize; i++)
      {
        m_cells[i] = CellArray(new Cell[m_ysize], [](auto ptr) { delete[] ptr; });
      }
    }
    else
    {
      std::cerr << __func__ << "Invalid size: " << "m_xsize= " << m_xsize << " m_ysize= " << m_ysize << " - resetting " << std::endl;
      m_xsize = m_ysize = 0;
      m_cells.reset();
    }
	}
	for (int x=0; x<m_xsize; x++)
  {
    for (int y = 0; y < m_ysize; y++)
    {
      m_cells[x][y] = g.m_cells[x][y];
    }
  }
	
	if (debug)
  {
		std::cerr << __func__ << std::endl;
		std::cerr << "m_xsize= " << m_xsize<< std::endl;
		std::cerr << "m_ysize= " << m_ysize<< std::endl;
	}
	return *this;
}

template <class Cell, const bool debug>
Array2D<Cell,debug>::Array2D(const Array2D<Cell,debug> & g)
{
  // delete memory
  if (m_cells)
  {
    m_cells.reset();
  }
	m_xsize=g.m_xsize;
	m_ysize=g.m_ysize;
  if (m_xsize > 0 && m_ysize > 0)
  {
    m_cells = CellArray2d(new CellArray[m_xsize], [](auto ptr) { delete[] ptr; });
    for (int x = 0; x < m_xsize; x++)
    {
      m_cells[x] = CellArray(new Cell[m_ysize], [](auto ptr) { delete[] ptr; });
      for (int y = 0; y < m_ysize; y++)
      {
        m_cells[x][y] = g.m_cells[x][y];
      }
    }
  }
  else
  {
    std::cerr << __func__ << "Invalid size: " << "m_xsize= " << m_xsize << " m_ysize= " << m_ysize << " - resetting " << std::endl;
    m_xsize=m_ysize=0;
    m_cells.reset();
  }
	if (debug){
		std::cerr << __func__ << std::endl;
		std::cerr << "m_xsize= " << m_xsize<< std::endl;
		std::cerr << "m_ysize= " << m_ysize<< std::endl;
	}
}

template <class Cell, const bool debug>
Array2D<Cell,debug>::~Array2D(){
  if (debug){
  	std::cerr << __func__ << std::endl;
    std::cerr << "m_xsize= " << m_xsize<< std::endl;
    std::cerr << "m_ysize= " << m_ysize<< std::endl;
  }
  // delete memory
  if (m_cells)
  {
    m_cells.reset();
  }
}

template <class Cell, const bool debug>
void Array2D<Cell,debug>::clear(){
  if (debug){
  	std::cerr << __func__ << std::endl;
    std::cerr << "m_xsize= " << m_xsize<< std::endl;
    std::cerr << "m_ysize= " << m_ysize<< std::endl;
  }
  // delete memory
  if (m_cells)
  {
    m_cells.reset();
  }
  m_xsize=0;
  m_ysize=0;
}


template <class Cell, const bool debug>
void Array2D<Cell,debug>::resize(int xmin, int ymin, int xmax, int ymax){
	int xsize = xmax-xmin;
	int ysize = ymax-ymin;
  if (xsize <= 0 || ysize <= 0)
  {
    std::cerr << __func__ << "Invalid reset size: " << "xsize= " << xsize << " ysize= " << ysize << " - no-op " << std::endl;
    return;
  }
  CellArray2d newcells=CellArray2d(new CellArray[xsize], [](auto ptr){ delete[] ptr; });
	for (int x=0; x<xsize; x++){
    newcells[x]=CellArray(new Cell[ysize], [](auto ptr){ delete[] ptr; });;
	}
	int dx= xmin < 0 ? 0 : xmin;
	int dy= ymin < 0 ? 0 : ymin;
	int Dx=xmax<this->m_xsize?xmax:this->m_xsize;
	int Dy=ymax<this->m_ysize?ymax:this->m_ysize;
	for (int x=dx; x<Dx; x++){
		for (int y=dy; y<Dy; y++){
			newcells[x-xmin][y-ymin]=this->m_cells[x][y];
		}
	}
	this->m_cells=newcells;
	this->m_xsize=xsize;
	this->m_ysize=ysize; 
}

template <class Cell, const bool debug>
inline bool Array2D<Cell,debug>::isInside(int x, int y) const{
	return x>=0 && y>=0 && x<m_xsize && y<m_ysize; 
}

template <class Cell, const bool debug>
inline const Cell& Array2D<Cell,debug>::cell(int x, int y) const{
	assert(isInside(x,y));
	return m_cells[x][y];
}


template <class Cell, const bool debug>
inline Cell& Array2D<Cell,debug>::cell(int x, int y){
	assert(isInside(x,y));
	return m_cells[x][y];
}

};

#endif

