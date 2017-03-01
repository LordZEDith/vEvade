#ifndef Core_h__
#define Core_h__

#include "Path.h"
#include "PathFollow.h"
#include "Polygons.h"

#include <map>
#include <queue>
#include <unordered_set>

void ProjectPointOn(Vec2 const& Point, Vec2 const& SegmentStart, Vec2 const& SegmentEnd, ProjectionInfo& Out);

namespace Core
{
	struct Node
	{
		std::vector<Node> Neighbours;
		Vec2 Point;

		Node(Vec2 point)
		{
			Point = point;
			Neighbours.clear();
		}

		Node()
		{
			Neighbours.clear();
		}
	};

	template<typename T1, typename T2>
	struct PriorityQueue
	{
		std::map<T1, std::queue<T2>> list;

		bool IsEmpty()
		{
			return (list.size() != 0);
		}

		T2 Dequeue()
		{
			auto pair = list.begin();
			auto deq = pair->second.front();

			pair->second.pop();

			if (deq.size() == 0)
				list.erase(pair->first);

			return deq;
		}

		void Enqueue(T1 p, T2 v)
		{
			std::queue<T2> q;

			if (list.find(p) == list.end())
				list[p] = q;

			q.push(v);
		}
	};

	static Vec2 GetClosestOutsidePoint(Vec2 from, std::vector<Geometry::IPolygon> polys)
	{
		auto result = std::vector<Vec2>();

		for (auto i : polys)
		{
			for (size_t j = 0; j <= i.Points.size() - 1; j++)
			{
				ProjectionInfo piout;
				ProjectPointOn(from, i.Points[j], i.Points[j == i.Points.size() - 1 ? 0 : j + 1], piout);

				result.push_back(piout.SegmentPoint);
			}
		}

		if (result.size() > 0)
		{
			float flClosestDistance = result.front().DistanceTo(from);
			Vec2 vecClosest = result.front();

			for (auto i : result)
			{
				float flDistance = i.DistanceTo(from);

				if (flDistance < flClosestDistance)
				{
					flDistance = flClosestDistance;
					vecClosest = i;
				}
			}

			return vecClosest;
		}

		return Vec2(0.f, 0.f);
	}

	static bool IsCross(Vec2 a, Vec2 b, Vec2 c, Vec2 d)
	{
		auto f = (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);

		if (f == 0.f)
			return false;

		auto num1 = (a.y - c.y) * (d.x - c.x) - (a.x - c.x) * (d.y - c.y);
		auto num2 = (a.y - c.y) * (b.x - a.x) - (a.x - c.x) * (b.y - a.y);

		if (num1 == 0.f || num2 == 0.f)
			return false;

		auto r = num1 / f;
		auto s = num2 / f;

		return r > 0 && r < 1 && s > 0 && s < 1;
	}

	static bool CanReach(Vec2 start, Vec2 end, std::vector<Geometry::IPolygon> polys)
	{
		if (start == end)
			return false;

		auto step = start.DistanceTo(end) / 2 * (end - start).VectorNormalize();

		for (auto i = 0; i <= 2; i++)
		{
			auto tmp = (start + i * step);

			if (GPrediction->IsPointWall(Vec3(tmp.x, GEntityList->Player()->GetPosition().y, tmp.y)))
				return false;
		}

		for (auto poly : polys)
		{
			for (size_t i = 0; i < poly.Points.size(); i++)
			{
				if (IsCross(start, end, poly.Points[i], poly.Points[i == poly.Points.size() - 1 ? 0 : i + 1]))
					return false;
			}
		}

		return true;
	}

	static bool IsConcave(std::vector<Vec2> v, int id)
	{
		auto cur = v[id];
		auto next = v[(id + 1) % v.size()];
		auto prev = v[id == 0 ? v.size() - 1 : id - 1];
		auto left = Vec2(cur.x - prev.x, cur.y - prev.y);
		auto right = Vec2(next.x - cur.x, next.y - cur.y);

		return left.x * right.y - left.y * right.x < 0;
	}

	static Path<Node> FindPath(Node start, Node dest, std::function<float(Node, Node, double)> dist, std::function<float(double)> eta)
	{
// 		std::unordered_set<Vec2> closed;
// 		auto queue = PriorityQueue<double, Path<Node>>();
// 
// 		while (!queue.IsEmpty())
// 		{
// 			auto path = queue.Dequeue();
// 
// 			if (std::find_if(closed.begin(), closed.end(), [&](Vec2 Args)
// 			{
// 				return (Args == path.LastStep.Point);
// 			}) == closed.end())
// 			{
// 				continue;
// 			}
// 
// 			if (path.LastStep.Point == dest.Point)
// 				return path;
// 			
// 			closed.emplace(path.LastStep.Point);
// 
// 			for (auto node : path.LastStep.Neighbours)
// 			{
// 				auto newPath = path.AddStep(node, dist(path.LastStep, node));
// 				queue.Enqueue(newPath.TotalCost + eta(node), newPath);
// 			}
// 		}
// 		
// 		return Path<Node>();
	}
	
	static std::vector<Vec2> FindPaths(Vec2 start, Vec2 end)
	{
		auto result = std::vector<Vec2>();

		auto outerPolys = std::vector<Geometry::IPolygon>();
		auto innerPolys = std::vector<Geometry::IPolygon>();

		for (auto i : Evade::Spells)
		{
			outerPolys.push_back(i->PathFindingOuterPolygon);
			innerPolys.push_back(i->PathFindingInnerPolygon);
		}

		auto outerPolygons = Geometry::ToPolygons(Geometry::ClipPolygons(outerPolys));
		auto innerPolygons = Geometry::ToPolygons(Geometry::ClipPolygons(innerPolys));

		for (auto i : outerPolygons)
		{
			if (i.IsInside(end))
				end = GetClosestOutsidePoint(end, outerPolygons);
		}

		for (auto i : outerPolygons)
		{
			if (i.IsInside(start))
				start = GetClosestOutsidePoint(start, outerPolygons);
		}

		if (CanReach(start, end, innerPolygons))
			return std::vector<Vec2>({ start, end });

		Geometry::IPolygon p2;
		p2.Points = std::vector<Vec2>({ start, end });

		outerPolygons.push_back(p2);

		auto nodes = std::vector<Node>();

		for (auto poly1 : outerPolygons)
		{
			for (size_t i = 0; i < poly1.Points.size(); i++)
			{
				if (poly1.Points.size() != 2 && IsConcave(poly1.Points, i))
					continue;

				auto& node1 = std::find_if(nodes.begin(), nodes.end(), [&](Node Args)
				{
					return (Args.Point == poly1.Points[i]);
				});

				if (node1 != nodes.end())
				{
					nodes.push_back((*node1));
				}
				else
				{
					Node newnode = Node(poly1.Points[i]);
					nodes.push_back(newnode);
				}

				for (auto poly2 : outerPolygons)
				{
					for (auto point : poly2.Points)
					{
						if (!CanReach(poly1.Points[i], point, innerPolygons))
							continue;

						auto& node2 = std::find_if(nodes.begin(), nodes.end(), [&](Node Args)
						{
							return (Args.Point == point);
						});

						if (node2 != nodes.end())
						{
							(*node2).Neighbours.push_back((*node2));
							nodes.push_back((*node2));
						}
						else
						{
							Node node2 = Node(point);
							nodes.back().Neighbours.push_back(node2);
							nodes.push_back(node2);
						}
					}
				}
			}

			auto& startNode = std::find_if(nodes.begin(), nodes.end(), [&](Node Args)
			{
				return Args.Point == start;
			});

			auto& endNode = std::find_if(nodes.begin(), nodes.end(), [&](Node Args)
			{
				return Args.Point == end;
			});

			if (endNode == nodes.end())
				return result;

			// 				Func<Node, Node, double> dist = (node1, node2) = > node1.Point.Distance(node2.Point);
			// 				Func<Node, double> eta = t = > t.Point.Distance(endNode.Point);
			// 				var path = startNode.FindPath(endNode, dist, eta);
			// 
			// 				if (path == null)
			// 				{
			// 					return result;
			// 				}
			// 
			// 				result.AddRange(path.Select(i = > i.Point));
			// 				result.Reverse();
		}
		// 
		// 			auto startNode = nodes.FirstOrDefault(i = > i.Point == start);
		// 			auto endNode = nodes.FirstOrDefault(i = > i.Point == end);
		// 
		// 			if (endNode == null)
		// 			{
		// 				return result;
		// 			}
		// 
		// 			Func<Node, Node, double> dist = (node1, node2) = > node1.Point.Distance(node2.Point);
		// 			Func<Node, double> eta = t = > t.Point.Distance(endNode.Point);
		// 			auto path = startNode.FindPath(endNode, dist, eta);
		// 
		// 			if (path == null)
		// 			{
		// 				return result;
		// 			}
		// 
		// 			result.AddRange(path.Select(i = > i.Point));
		// 			result.Reverse();
		// 		}
		// 		catch (...)
		// 		{
		// 			GGame->PrintChat("vEvade Exception: Core::FindPaths()");
		// 		}
		// 
		// 		return result;
		// 	}
		// 
		// 
		// 	private static Path<Node> FindPath(
		// 		this Node start,
		// 		Node dest,
		// 		Func<Node, Node, double> dist,
		// 		Func<Node, double> eta)
		// 	{
		// 		var closed = new HashSet<Vector2>();
		// 		var queue = new PriorityQueue<double, Path<Node>>();
		// 		queue.Enqueue(0, new Path<Node>(start));
		// 
		// 		while (!queue.IsEmpty)
		// 		{
		// 			var path = queue.Dequeue();
		// 
		// 			if (closed.Contains(path.LastStep.Point))
		// 			{
		// 				continue;
		// 			}
		// 
		// 			if (path.LastStep.Point.Equals(dest.Point))
		// 			{
		// 				return path;
		// 			}
		// 
		// 			closed.Add(path.LastStep.Point);
		// 
		// 			foreach(var node in path.LastStep.Neightbours)
		// 			{
		// 				var newPath = path.AddStep(node, dist(path.LastStep, node));
		// 				queue.Enqueue(newPath.TotalCost + eta(node), newPath);
		// 			}
		// 		}
		// 
		// 		return null;
		// 	}
		// 
		// 	private static Vector2 GetClosestOutsidePoint(this Vector2 from, List<Geometry.Polygon> polys)
		// 	{
		// 		var result = new List<Vector2>();
		// 
		// 		foreach(var poly in polys)
		// 		{
		// 			for (var i = 0; i <= poly.Points.Count - 1; i++)
		// 			{
		// 				result.Add(
		// 					from.ProjectOn(poly.Points[i], poly.Points[i == poly.Points.Count - 1 ? 0 : i + 1]).SegmentPoint);
		// 			}
		// 		}
		// 
		// 		return result.Count > 0 ? result.OrderBy(i = > i.Distance(from)).First() : Vector2.Zero;
		// 	}
		// 
		// 	private static bool IsConcave(this List<Vector2> v, int id)
		// 	{
		// 		var cur = v[id];
		// 		var next = v[(id + 1) % v.Count];
		// 		var prev = v[id == 0 ? v.Count - 1 : id - 1];
		// 		var left = new Vector2(cur.X - prev.X, cur.Y - prev.Y);
		// 		var right = new Vector2(next.X - cur.X, next.Y - cur.Y);
		// 
		// 		return left.X * right.Y - left.Y * right.X < 0;
		// 	}
		// 
		// 	private static bool IsCross(this Vector2 a, Vector2 b, Vector2 c, Vector2 d)
		// 	{
		// 		var f = (b.X - a.X) * (d.Y - c.Y) - (b.Y - a.Y) * (d.X - c.X);
		// 
		// 		if (f.Equals(0f))
		// 		{
		// 			return false;
		// 		}
		// 
		// 		var num1 = (a.Y - c.Y) * (d.X - c.X) - (a.X - c.X) * (d.Y - c.Y);
		// 		var num2 = (a.Y - c.Y) * (b.X - a.X) - (a.X - c.X) * (b.Y - a.Y);
		// 
		// 		if (num1.Equals(0f) || num2.Equals(0f))
		// 		{
		// 			return false;
		// 		}
		// 
		// 		var r = num1 / f;
		// 		var s = num2 / f;
		// 
		// 		return r > 0 && r < 1 && s > 0 && s < 1;
		// 	}
		// 
		// 	public class PriorityQueue<TP, TV>
		// 	{
		// 		#region Fields
		// 
		// 		private readonly SortedDictionary<TP, Queue<TV>> list = new SortedDictionary<TP, Queue<TV>>();
		// 
		// 		#endregion
		// 
		// 		#region Public Properties
		// 
		// 		public bool IsEmpty = > !this.list.Any();
		// 
		// 		#endregion
		// 
		// 		#region Public Methods and Operators
		// 
		// 		public TV Dequeue()
		// 		{
		// 			var pair = this.list.First();
		// 			var deq = pair.Value.Dequeue();
		// 
		// 			if (pair.Value.Count == 0)
		// 			{
		// 				this.list.Remove(pair.Key);
		// 			}
		// 
		// 			return deq;
		// 		}
		// 
		// 		public void Enqueue(TP p, TV v)
		// 		{
		// 			Queue<TV> q;
		// 
		// 			if (!this.list.TryGetValue(p, out q))
		// 			{
		// 				q = new Queue<TV>();
		// 				this.list.Add(p, q);
		// 			}
		// 
		// 			q.Enqueue(v);
		// 		}
		// 
		// 		#endregion
		// 	}
	}
}

#endif // Core_h__
