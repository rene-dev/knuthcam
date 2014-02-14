#include "DxfParser.hpp"

using std::cout;
using std::endl;
using glm::vec2;

//int DxfParser::open(string s, drawing_t &d){
//	drawing = &d;
//    drawing->min = vec2(10,10);
//    drawing->max = vec2(10,10);
//	return dxf.in(s,this);
//}
//
//
//int DxfParser::save(string s, drawing_t &d){
//	cout << "dxf export not implemented!" << endl;
//	return(0);
//}
//
//void DxfParser::processCodeValuePair(unsigned int i, char* c) {
//	//cout << __func__ << " " << i <<": " << c << endl;
//}
//void DxfParser::addLayer(const DL_LayerData& d) {
//	//cout << __func__ << " " << d.name << endl;
//	layer_t l;
//	l.name = d.name;
//	drawing->layers.push_back(l);
//}
////void DxfParser::addBlock(const DL_BlockData& b) {cout << __func__ << " " << b.name << endl;}
////void DxfParser::endBlock() {cout << __func__ << endl;}
//
//void DxfParser::minmax (vec2 v){
//    drawing->min.x = fmin(drawing->min.x,v.x);
//    drawing->min.y = fmin(drawing->min.y,v.y);
//    drawing->max.x = fmax(drawing->max.x,v.x);
//    drawing->max.y = fmax(drawing->max.y,v.y);
//}
//
//void DxfParser::addPoint(const DL_PointData&) {cout << __func__ << endl;}
//void DxfParser::addLine(const DL_LineData& d) {
//	//cout << __func__ << endl;
//    DL_Attributes a = getAttributes();
//	string layer = a.getLayer();
//	seg_line s(vec2(d.x1,d.y1), vec2(d.x2,d.y2));
//    //TODO: klappt noch nicht
//    minmax(s.start);
//    minmax(s.end);
//	for(layer_t &l : drawing->layers){
//		if(l.name == layer){
//			l.segments.segments.push_back(s);
//			return;
//		}
//	}
//	//cout << "layer " << layer << " not found" << endl;        
//	//cout << "line on layer " << a.getLayer() << " from " << d.x1 << "," << d.y1 << " to " << d.x2 << "," << d.y2 << endl;
//}
//void DxfParser::addArc(const DL_ArcData& d) {
//	//cout << __func__ << endl;
//	//cout << d.cx << "," << d.cy << " r:" << d.radius << " start:" << d.angle1 << " end:" << d.angle2 << " " << endl;
//	DL_Attributes a = getAttributes();
//	string layer = a.getLayer();
//	seg_ccw_arc s;
//    
//	s.mid = vec2(d.cx,d.cy);
//
//	vec2 t;
//	t.x = cos(d.angle1 / 180.0 * M_PI) * d.radius;
//	t.y = sin(d.angle1 / 180.0 * M_PI) * d.radius;
//	s.start = s.mid + t;
//
//	t.x = cos(d.angle2 / 180.0 * M_PI) * d.radius;
//	t.y = sin(d.angle2 / 180.0 * M_PI) * d.radius;
//	s.end = s.mid + t;
//
//	for(layer_t &l : drawing->layers){
//		if(l.name == layer){
//			l.segments.segments.push_back(s);
//			return;
//		}
//	}
//
//}
//
//void DxfParser::addCircle(const DL_CircleData&) {cout << __func__ << endl;}
//void DxfParser::addEllipse(const DL_EllipseData&) {cout << __func__ << endl;}
//void DxfParser::addPolyline(const DL_PolylineData&) {cout << __func__ << endl;}
//void DxfParser::addVertex(const DL_VertexData&) {cout << __func__ << endl;}
//void DxfParser::addSpline(const DL_SplineData&) {cout << __func__ << endl;}
//void DxfParser::addcont_trolPoint(const DL_ControlPointData&) {cout << __func__ << endl;}
//void DxfParser::addKnot(const DL_KnotData&) {cout << __func__ << endl;}
//void DxfParser::addInsert(const DL_InsertData&) {cout << __func__ << endl;}
//void DxfParser::addMText(const DL_MTextData&) {cout << __func__ << endl;}
//void DxfParser::addMTextChunk(const char*) {cout << __func__ << endl;}
//void DxfParser::addText(const DL_TextData&) {cout << __func__ << endl;}
//void DxfParser::addDimAlign(const DL_DimensionData&,const DL_DimAlignedData&) {cout << __func__ << endl;}
//void DxfParser::addDimLinear(const DL_DimensionData&,const DL_DimLinearData&) {cout << __func__ << endl;}
//void DxfParser::addDimRadial(const DL_DimensionData&,const DL_DimRadialData&) {cout << __func__ << endl;}
//void DxfParser::addDimDiametric(const DL_DimensionData&,const DL_DimDiametricData&) {cout << __func__ << endl;}
//void DxfParser::addDimAngular(const DL_DimensionData&,const DL_DimAngularData&) {cout << __func__ << endl;}
//void DxfParser::addDimAngular3P(const DL_DimensionData&,const DL_DimAngular3PData&) {cout << __func__ << endl;}
//void DxfParser::addDimOrdinate(const DL_DimensionData&,const DL_DimOrdinateData&) {cout << __func__ << endl;}
//void DxfParser::addLeader(const DL_LeaderData&) {cout << __func__ << endl;}
//void DxfParser::addLeaderVertex(const DL_LeaderVertexData&) {cout << __func__ << endl;}
//void DxfParser::addHatch(const DL_HatchData&) {cout << __func__ << endl;}
//void DxfParser::addTrace(const DL_TraceData&) {cout << __func__ << endl;}
//void DxfParser::add3dFace(const DL_3dFaceData&) {cout << __func__ << endl;}
//void DxfParser::addSolid(const DL_SolidData&) {cout << __func__ << endl;}
//void DxfParser::addImage(const DL_ImageData&) {cout << __func__ << endl;}
//void DxfParser::linkImage(const DL_ImageDefData&) {cout << __func__ << endl;}
//void DxfParser::addHatchLoop(const DL_HatchLoopData&) {cout << __func__ << endl;}
//void DxfParser::addHatchEdge(const DL_HatchEdgeData&) {cout << __func__ << endl;}
//void DxfParser::endEntity() {cout << __func__ << endl;}
////void DxfParser::addComment(const char* comment) {cout << "comment: " << comment << endl;}
////void DxfParser::setVariableVector(const char* c, double, double, double, int) {cout << __func__ << " " << c << endl;}
////void DxfParser::setVariableString(const char* c, const char*, int) {cout << __func__ << " " << c << endl;}
////void DxfParser::setVariableInt(const char* c, int, int) {cout << __func__ << " " << c << endl;}
////void DxfParser::setVariableDouble(const char* c, double, int) {cout << __func__ << " " << c << endl;}
//void DxfParser::endSequence() {cout << __func__ << endl;}
