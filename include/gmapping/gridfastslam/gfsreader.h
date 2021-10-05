#ifndef GFSREADER_H
#define GFSREADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <gmapping/utils/point.h>

#define MAX_LINE_LENGHT (1000000)

namespace GMapping{

namespace GFSReader{
	
using namespace std;

struct Record{
	unsigned int dim;
	double time;
	virtual ~Record();
	virtual void read(istream& is)=0;
	virtual void write(ostream& os);
};

struct CommentRecord: public Record{
	string text;
	virtual void read(istream& is);
	virtual void write(ostream& os);
};

struct PoseRecord: public Record{
	PoseRecord(bool ideal=false);
	void read(istream& is);
	virtual void write(ostream& os);
	bool truePos;
	OrientedPoint pose;
};

struct NeffRecord: public Record{
	void read(istream& is);
	virtual void write(ostream& os);
	double neff;
};

struct EntropyRecord: public Record{
	void read(istream& is);
	virtual void write(ostream& os);
	double poseEntropy;
	double trajectoryEntropy;
	double mapEntropy;
};


struct OdometryRecord: public Record{
	virtual void read(istream& is);
	vector<OrientedPoint> poses;
};

struct RawOdometryRecord: public Record{
	virtual void read(istream& is);
	OrientedPoint pose;
};

struct ScanMatchRecord: public Record{
	virtual void read(istream& is);
	vector<OrientedPoint> poses;
	vector<double> weights;
};

struct LaserRecord: public Record{
	virtual void read(istream& is);
	virtual void write(ostream& os);
	vector<double> readings;
	OrientedPoint pose;
	double weight;
};

struct ResampleRecord: public Record{
	virtual void read(istream& is);
	vector<unsigned int> indexes;
};

struct RecordList: public list<Record*>{
	mutable int sampleSize;
	istream& read(istream& is);
	double getLogWeight(unsigned int i) const;
	double getLogWeight(unsigned int i, RecordList::const_iterator frame) const;
	unsigned int getBestIdx() const ;
	void printLastParticles(ostream& os) const ;
	void printPath(ostream& os, unsigned int i, bool err=false, bool rawodom=false) const;
	RecordList computePath(unsigned int i, RecordList::const_iterator frame) const;
	void destroyReferences();
};

}; //end namespace GFSReader

}; //end namespace GMapping

#endif
