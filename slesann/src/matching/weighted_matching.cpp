/*
 * weighted_matching.cpp
 *
 * Copyright (C) 2011 Leo Osvald <leo.osvald@gmail.com>
 *
 * This file is part of SLESANN.
 *
 * SLESANN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SLESANN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SLESANN. If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * weighted_matching.cpp
 *
 *  Created on: Mar 1, 2011
 *      Author: losvald
 */

//#include "weighted_matching.h"
//
//MyEdge::MyEdge(int a, int b, int w) : a(a), b(b), w(w) { }
////	friend bool operator<(const MyEdge& A, const MyEdge& B) {
////		if(A.a != B.a)
////			return A.a < B.a;
////		if(B.a != B.b)
////			return B.a < B.b;
////		return A.w < B.w;
////	}
////	friend bool operator==(const MyEdge& A, const MyEdge& B) {
////		return A.a == B.a && A.b == B.b && A.w == B.w;
////	}
//
//std::ostream& operator<<(std::ostream& os, const MyEdge& e) {
//  os << e.a << " " << e.b << " " << e.w;
//  return os;
//}
//
//namespace boost {
//
//template <class Graph, class CapacityMap, class ReverseEdgeMap>
//bool Construct(Graph& g,
//               CapacityMap capacity,
//               ReverseEdgeMap reverse_edge,
//               typename graph_traits<Graph>::vertex_descriptor& src,
//               typename graph_traits<Graph>::vertex_descriptor& sink,
//               const std::vector<MyEdge>& _edges, int _src, int _sink) {
//  typedef typename graph_traits<Graph>::vertices_size_type vertices_size_type;
//  typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
//  typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
//
//  int n = std::max(_src, _sink);
//  for(int i = 0; i < (int)_edges.size(); ++i) {
//    n = std::max(n, std::max(_edges[i].a, _edges[i].b));
//  }
//  ++n;
//  //create vertices
//  std::vector<vertex_descriptor> verts;
//  for (long vi = 0; vi < n; ++vi)
//    verts.push_back(add_vertex(g));
//
//  src = verts[_src];
//  sink = verts[_sink];
//
//  //create edges
//  for(int i = 0; i < (int)_edges.size(); ++i) {
//    edge_descriptor e1, e2;
//    bool in1, in2;
//    tie(e1, in1) = add_edge(verts[_edges[i].a], verts[_edges[i].b], g);
//    tie(e2, in2) = add_edge(verts[_edges[i].b], verts[_edges[i].a], g);
//    if (!in1 || !in2) {
//      std::cerr << "unable to add edge (" << _edges[i].a << ","
//          << _edges[i].b << ")"
//          << std::endl;
//      return false;
//    }
//    capacity[e1] = _edges[i].w;
//    capacity[e2] = 0;
//    reverse_edge[e1] = e2;
//    reverse_edge[e2] = e1;
//  }
//
//  return true;
//
//}
//
//}
//
//int FindMaxFlow(const std::vector<MyEdge>& _edges, int _src, int _sink,
//                bool print_graph) {
//  using namespace boost;
//  typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
//  typedef adjacency_list<vecS, vecS, directedS,
//      property<vertex_name_t, std::string>,
//      property<edge_capacity_t, long,
//      property<edge_residual_capacity_t, long,
//      property<edge_reverse_t, Traits::edge_descriptor> > >
//  > Graph;
//
//  Graph g;
//
//  property_map<Graph, edge_capacity_t>::type
//  capacity = get(edge_capacity, g);
//  property_map<Graph, edge_reverse_t>::type
//  rev = get(edge_reverse, g);
//  property_map<Graph, edge_residual_capacity_t>::type
//  residual_capacity = get(edge_residual_capacity, g);
//
//  Traits::vertex_descriptor s, t;
//  //  read_dimacs_max_flow(g, capacity, rev, s, t);
//  Construct(g, capacity, rev, s, t, _edges, _src, _sink);
//
//  long flow = push_relabel_max_flow(g, s, t);
//
//  if(print_graph) {
//    std::cout << "c  The total flow:" << std::endl;
//    std::cout << "s " << flow << std::endl << std::endl;
//    std::cout << "c flow values:" << std::endl;
//    graph_traits<Graph>::vertex_iterator u_iter, u_end;
//    graph_traits<Graph>::out_edge_iterator ei, e_end;
//    for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter)
//      for (tie(ei, e_end) = out_edges(*u_iter, g); ei != e_end; ++ei)
//        if (capacity[*ei] > 0)
//          std::cout << "f " << *u_iter << " " << target(*ei, g) << " "
//          << (capacity[*ei] - residual_capacity[*ei]) << std::endl;
//  }
//
//  return flow;
//}

