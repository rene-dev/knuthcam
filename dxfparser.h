#ifndef dxfparser_h
#define dxfparser_h

#include <iostream>
#include <string>
#include <vector>
#include <dxflib/dl_dxf.h>
#include <dxflib/dl_creationadapter.h>
#include <glm/glm.hpp>

#include "knuthcam.h"
#include "importexport.h"

class DxfParser : public DL_CreationAdapter, importexport{
	public:
		drawing_t *drawing;
		int open(string s, drawing_t &d);
		int save(string s, drawing_t &d);
	private:
		DL_Dxf dxf;
        void minmax (vec2);
		void processCodeValuePair(unsigned int, char*);
		void addLayer(const DL_LayerData&);
		//void addBlock(const DL_BlockData&);
		//void endBlock();
		void addPoint(const DL_PointData&);
		void addLine(const DL_LineData&);
		void addArc(const DL_ArcData&);
		void addCircle(const DL_CircleData&);
		void addEllipse(const DL_EllipseData&);

		void addPolyline(const DL_PolylineData&);
		void addVertex(const DL_VertexData&);

		void addSpline(const DL_SplineData&);
		void addControlPoint(const DL_ControlPointData&);
		void addKnot(const DL_KnotData&);

		void addInsert(const DL_InsertData&);

		void addMText(const DL_MTextData&);
		void addMTextChunk(const char*);
		void addText(const DL_TextData&);

		void addDimAlign(const DL_DimensionData&,const DL_DimAlignedData&);
		void addDimLinear(const DL_DimensionData&,const DL_DimLinearData&);
		void addDimRadial(const DL_DimensionData&,const DL_DimRadialData&);
		void addDimDiametric(const DL_DimensionData&,const DL_DimDiametricData&);
		void addDimAngular(const DL_DimensionData&,const DL_DimAngularData&);
		void addDimAngular3P(const DL_DimensionData&,const DL_DimAngular3PData&);
		void addDimOrdinate(const DL_DimensionData&,const DL_DimOrdinateData&);
		void addLeader(const DL_LeaderData&);
		void addLeaderVertex(const DL_LeaderVertexData&);

		void addHatch(const DL_HatchData&);

		void addTrace(const DL_TraceData&);
		void add3dFace(const DL_3dFaceData&);
		void addSolid(const DL_SolidData&);

		void addImage(const DL_ImageData&);
		void linkImage(const DL_ImageDefData&);
		void addHatchLoop(const DL_HatchLoopData&);
		void addHatchEdge(const DL_HatchEdgeData&);
		void endEntity();
		//void addComment(const char* comment);
		//void setVariableVector(const char*, double, double, double, int);
		//void setVariableString(const char*, const char*, int);
		//void setVariableInt(const char*, int, int);
		//void setVariableDouble(const char*, double, int);
		void endSequence();
};

#endif
