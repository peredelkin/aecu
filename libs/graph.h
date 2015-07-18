/* 
 * File:   graph.h
 * Author: root
 *
 * Created on 3 ноября 2014 г., 19:55
 */

#ifndef GRAPH_H
#define	GRAPH_H

#include "graphics.h"

typedef struct {
    uint16_t point_val[128];
    uint8_t point;
    uint8_t draw_point;
} graph_t;

extern void set_ygraph(graph_t* graph,uint8_t from,uint8_t to,uint8_t param);
extern void draw_ygraph(graph_t* graph, graphics_t* graphics,uint8_t from,uint8_t to);
extern void init_ygraph(graph_t* graph, uint8_t param);
extern void draw_xgraph_inverted(graph_t* graph, graphics_t* graphics,uint8_t from,uint8_t to,uint8_t param);
extern void draw_xgraph(graph_t* graph, graphics_t* graphics,uint8_t from,uint8_t to,uint8_t param,uint8_t order,int8_t shift);
extern void init_xgraph(graph_t* graph, uint8_t param);

#endif	/* GRAPH_H */


