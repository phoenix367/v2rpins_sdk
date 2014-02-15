/* 
 * File:   GraphPlot.hpp
 * Author: Ivan Gubochkin
 *
 * Created on February 15, 2014, 11:26 PM
 */

#ifndef GRAPHPLOT_HPP
#define	GRAPHPLOT_HPP

#include "qwt3d_plot.h"

class GraphPlot : public Qwt3D::VertexEnrichment
{
public:
    GraphPlot();
    virtual ~GraphPlot();
    
    Qwt3D::Enrichment* clone() const 
    {
        return new GraphPlot(*this);
    }

    void drawBegin();
    void drawEnd();
    void draw(Qwt3D::Triple const&);

private:

};

#endif	/* GRAPHPLOT_HPP */
