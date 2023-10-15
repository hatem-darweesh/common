
/// \file Lanelet2MapLoader
/// \brief Functions for Loading Lanelet2 .osm map file format.
/// \author Hatem Darweesh
/// \date Mar 17, 2020

#ifndef LANELET2MAPLOADER_H_
#define LANELET2MAPLOADER_H_

#include <op_planner/MappingHelpers.h>

#include <lanelet2_io/Io.h>
#include <lanelet2_core/primitives/Lanelet.h>
#include <lanelet2_io/io_handlers/Factory.h>
#include <lanelet2_io/io_handlers/Writer.h>
#include <lanelet2_projection/UTM.h>
#include <lanelet2_routing/Route.h>
#include <lanelet2_routing/RoutingCost.h>
#include <lanelet2_routing/RoutingGraph.h>
#include <lanelet2_routing/RoutingGraphContainer.h>
#include <lanelet2_traffic_rules/TrafficRulesFactory.h>
#include <lanelet2_extension/utility/query.h>
#include <lanelet2_core/LaneletMap.h>
#include <lanelet2_extension/utility/message_conversion.h>
#include <lanelet2_extension/projection/mgrs_projector.h>
#include <lanelet2_extension/io/autoware_osm_parser.h>
#include <lanelet2_extension/utility/utilities.h>
#include <autoware_lanelet2_msgs/MapBin.h>

namespace PlannerHNS
{


class Lanelet2MapLoader {
public:
	Lanelet2MapLoader(bool bCalcWidthForEachPoint = false);
	virtual ~Lanelet2MapLoader();

	/**
	 * @brief Parsing the lanelet 2 message, which is loaded using autoware lanelet2 extention.
	 * @param msg, bin formated lanelet2 autoware message
	 * @param map, OpenPlanner Road Network map.
	 */
	void LoadMap(const autoware_lanelet2_msgs::MapBin& msg, PlannerHNS::RoadNetwork& map);

	/**
	 * @brief Reading lanelet2 .osm based map format, and convert it into OpenPlanner Road Network map.
	 * @param fileName, .osm map format, lanelet2
	 * @param map, OpenPlanner custom Road Network map format.
	 */
	lanelet::LaneletMapPtr LoadMap(const std::string& fileName, PlannerHNS::RoadNetwork& map);

private:
	lanelet::Projector* m_pProjector = nullptr;
	std::string m_ProjStr;
	lanelet::routing::RoutingGraphUPtr m_RoutingGraph;
	lanelet::LaneletMapPtr m_pL2Map;
	bool m_bCalcWidthForEachPoint = false;


	void FromLaneletToRoadNetwork(lanelet::LaneletMapPtr l2_map, PlannerHNS::RoadNetwork& map, lanelet::Projector* proj, lanelet::routing::RoutingGraphUPtr& routingGraph);
	void CreateWayPointsFromLineString(const PlannerHNS::RoadNetwork& map,std::vector<PlannerHNS::WayPoint>& points, lanelet::ConstLineString3d& line_string, lanelet::Projector* proj, int lane_id = 0);
	void CreateWayPointsFromLineString(const PlannerHNS::RoadNetwork& map,std::vector<PlannerHNS::WayPoint>& points, const lanelet::LineString3d& line_string, lanelet::Projector* proj, int lane_id = 0);
	void CreateWayPointsFromPolygon(const PlannerHNS::RoadNetwork& map, std::vector<PlannerHNS::WayPoint>& points,const lanelet::ConstPolygon3d& line_string, lanelet::Projector* proj, int lane_id = 0);
	std::vector<PlannerHNS::TrafficLight> CreateTrafficLightsFromLanelet2(const PlannerHNS::RoadNetwork& map, lanelet::AutowareTrafficLightConstPtr& tl_let, lanelet::Projector* proj, int lane_id = 0);
//	std::vector<PlannerHNS::StopLine> CreateStopLinesFromLanelet2(const PlannerHNS::RoadNetwork& map, lanelet::ConstLineString3d& sl_let, lanelet::Projector* proj, int lane_id = 0);
	void ExtractFirstLongLatFromFileAsOrigin(const std::string& fileName, PlannerHNS::RoadNetwork& map);
	void CreateLane(lanelet::routing::RoutingGraphUPtr& routingGraph, lanelet::ConstLanelet& lanelet_obj, PlannerHNS::Lane& l, PlannerHNS::RoadNetwork& map, lanelet::Projector* proj);


	bool ExtractLane(lanelet::ConstLanelet& let, PlannerHNS::Lane& l);
	bool ExtractSign(std::shared_ptr<const lanelet::TrafficSign>& ts, PlannerHNS::TrafficSign& op_sign);
	void ExtractWayPointsFromLineString(lanelet::ConstLineString3d& line_string, std::vector<WayPoint>& points);
	void ExtractWayPointsFromPolygon(lanelet::ConstPolygon3d& line_string, std::vector<WayPoint>& points);
	void ExtractRoadSegmentsFromLanelets(std::vector<RoadSegment>& roads);
	void ExtractSignsFromLanelets(RoadNetwork& map);
	void ExtractLightsFromLanelets(RoadNetwork& map);
	void ExtractStopLines(lanelet::ConstLineStrings3d& let_stop_lines, std::vector<StopLine>& stop_lines);
	void ExtractBoundariesFromLanelets(RoadNetwork& map);
	void ExtractMarkingsFromLanelets(RoadNetwork& map);
	void PrintExistingAttributes();

};

} /* namespace PlannerHNS */

#endif /* LANELET2MAPLOADER_H_ */
