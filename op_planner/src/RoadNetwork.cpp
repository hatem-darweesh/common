/*
 * RoadNetwork.cpp
 *
 *  Created on: Mar 16, 2020
 *      Author: hatem
 */

#include <op_planner/RoadNetwork.h>

namespace PlannerHNS
{

int RoadNetwork::g_max_point_id = 1;
int RoadNetwork::g_max_lane_id = 1;
int RoadNetwork::g_max_line_id = 1;
int RoadNetwork::g_max_stop_line_id = 1;
int RoadNetwork::g_max_traffic_light_id = 1;
int RoadNetwork::g_max_traffic_sign_id = 1;
int RoadNetwork::g_max_boundary_area_id = 1;
int RoadNetwork::g_max_marking_id = 1;
int RoadNetwork::g_max_curb_id = 1;
int RoadNetwork::g_max_crossing_id = 1;
int RoadNetwork::g_max_road_id = 1;
int RoadNetwork::g_max_junction_id = 1;


RoadNetwork& RoadNetwork::operator=(const RoadNetwork &map)
{
	this->roadSegments = map.roadSegments;
	this->lines = map.lines;
	this->crossings = map.crossings;
	this->markings = map.markings;
	this->boundaries = map.boundaries;
	this->curbs = map.curbs;
	this->junctions = map.junctions;
	this->g_max_point_id = map.g_max_point_id;
	this->g_max_lane_id = map.g_max_lane_id;
	this->g_max_line_id = map.g_max_line_id;
	this->g_max_stop_line_id = map.g_max_stop_line_id;
	this->g_max_traffic_light_id = map.g_max_traffic_light_id;
	this->g_max_traffic_sign_id = map.g_max_traffic_sign_id;
	this->g_max_boundary_area_id = map.g_max_boundary_area_id;
	this->g_max_marking_id = map.g_max_marking_id;
	this->g_max_curb_id = map.g_max_curb_id;
	this->g_max_crossing_id = map.g_max_crossing_id;
	this->g_max_road_id = map.g_max_road_id;
	this->g_max_junction_id = map.g_max_junction_id;
	this->origin = map.origin;
	this->proj = map.proj;
	this->str_proj = map.str_proj;
	this->signs = map.signs;
	this->stopLines = map.stopLines;
	this->bLeftHand = map.bLeftHand;
	this->trafficLights = map.trafficLights;

	for(auto& seg: roadSegments)
	{
		for(auto& l: seg.Lanes)
		{
			l.clearPointers();
		}
		seg.clearPointers();
	}

	for(auto& l: lines)
	{
		l.clearPointers();
	}

	for(auto& l: crossings)
	{
		l.clearPointers();
	}

	for(auto& l: markings)
	{
		l.clearPointers();
	}

	for(auto& l: signs)
	{
		l.clearPointers();
	}

	for(auto& l: boundaries)
	{
		l.clearPointers();
	}

	for(auto& l: curbs)
	{
		l.clearPointers();
	}

	for(auto& l: stopLines)
	{
		l.clearPointers();
	}

	return *this;
}

int RoadNetwork::GetNoOfLanes()
{
	int nLanes = 0;
	for(auto& seg: roadSegments)
	{
		nLanes += seg.Lanes.size();
	}
	return nLanes;
}

void RoadNetwork::ReplaceRoadIdWith(const OPID& old_id, const OPID& id)
{
	for(auto& seg: roadSegments)
	{
		if(seg.id == old_id) seg.id = id;
		for(auto& x: seg.toIds)
		{
			if(x == old_id) x = id;
		}

		for(auto& x: seg.fromIds)
		{
			if(x == old_id) x = id;
		}

		for(auto& l: seg.Lanes)
		{
			if(l.roadId == old_id) l.roadId = id;
		}
	}

	for(auto& l: lines)
	{
		if(l.roadID == old_id) l.roadID = id;
	}

	for(auto& l: crossings)
	{
		if(l.roadId == old_id) l.roadId = id;
	}

	for(auto& l: markings)
	{
		if(l.roadId == old_id) l.roadId = id;
	}

	for(auto& l: signs)
	{
		if(l.roadId == old_id) l.roadId = id;
	}

	for(auto& l: boundaries)
	{
		if(l.roadId == old_id) l.roadId = id;
	}

	for(auto& l: curbs)
	{
		if(l.roadId == old_id) l.roadId = id;
	}

	for(auto& l: stopLines)
	{
		if(l.roadId == old_id) l.roadId = id;
	}
}

Lane* RoadNetwork::GetLaneByWaypointId(const OPID& wp_id)
{
	for(auto& seg: roadSegments)
	{
		for(auto& l: seg.Lanes)
		{
			for(auto& p: l.points)
			{
				if(p.id == wp_id)
				{
					return &l;
				}
			}
		}
	}

	return nullptr;
}

WayPoint* RoadNetwork::GetWayPointById(const OPID& pointId)
{
	if(pointId <= 0) return nullptr;

	for(auto& seg: roadSegments)
	{
		for(auto& l: seg.Lanes) {
			WayPoint* pP = l.GetWayPointById(pointId);
			if(pP != nullptr) return pP;
		}
	}
	return nullptr;
}

Lane* RoadNetwork::GetLaneById(const OPID& laneId)
{
	if(laneId <= 0) return nullptr;

	for(auto& seg: roadSegments)
	{
		for(auto& l: seg.Lanes) {
			if(l.id == laneId) return &l;
		}
	}
	return nullptr;
}

RoadSegment* RoadNetwork::GetSegmentById(const OPID& roadId)
{
	if(roadId <= 0) return nullptr;

	for(auto& seg: roadSegments) {
		if(seg.id == roadId) return &seg;
	}

	return nullptr;
}

RoadSegment* RoadNetwork::GetSegmentByLaneId(const OPID& laneId)
{
	if(laneId <= 0) return nullptr;

	for(auto& seg: roadSegments) {
		for(auto& l: seg.Lanes) {
			if(l.id == laneId) return &seg;
		}
	}

	return nullptr;
}

Junction* RoadNetwork::FindJunction(const OPID& connectingRoadId, const OPID& incommingRoadId)
{
	for(auto& junc: junctions)
	{
		Connection* pConn = junc.FindConnection(connectingRoadId, incommingRoadId);

		if(pConn != nullptr)
		{
			return &junc;
		}
	}

	return nullptr;
}

}


