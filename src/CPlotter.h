#ifndef CPLOTTER_H
#define CPLOTTER_H
#include <QtGui/QHBoxLayout>
#include <QtGui/QWidget>

#include <qwt_plot.h>
#include <qwt_color_map.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_renderer.h>
#include <qwt_raster_data.h>


/* -------------------------------------------
 * SpectrogramData derived from QwtRasterData
 * Saves the values for the plot
 * -------------------------------------------
*/

class SpectrogramData: public QwtRasterData
{
private:
        double * m_Array;
        double m_minValue;
        double m_maxValue;

        struct structMinMax{
                double min;
                double max;
        };
        structMinMax m_RangeX;
        structMinMax m_RangeY;
        struct structXY{
                double x;
                double y;
        };
        structXY m_DataSize;
        structXY m_RealToArray;

public:
        // Constructor giving back the QwtRasterData Constructor
    SpectrogramData(): QwtRasterData()
    {
                m_Array = NULL;
    }

    ~SpectrogramData()
    {
                if (m_Array != NULL)
                        delete [] m_Array;
    }

    virtual QwtRasterData *copy() const
    {
        SpectrogramData *clone = new SpectrogramData();
                clone->setRangeX(m_RangeX.min, m_RangeX.max);
                clone->setRangeY(m_RangeY.min, m_RangeY.max);
                //clone->setBoundingRect(QwtDoubleRect(m_RangeX.min, m_RangeY.min, m_RangeX.max, m_RangeY.max));
                clone->setInterval(Qt::XAxis, QwtInterval( m_RangeX.min, m_RangeX.max));
                clone->setInterval(Qt::YAxis, QwtInterval( m_RangeY.min, m_RangeY.max));
                clone->setInterval(Qt::ZAxis, QwtInterval(0.0, 2.0));
                clone->setData(m_Array, m_DataSize.x, m_DataSize.y);
                return clone;
    }
/*
    virtual QwtInterval range() const
    {
        qDebug() << "Interval " << QwtInterval(m_minValue, m_maxValue);
        return QwtInterval(0.0, 2.0);
    }
*/
    virtual double value(double x, double y) const
    {
                int xpos = (int)((x - m_RangeX.min) / m_RealToArray.x);
                int ypos = (int)((y - m_RangeY.min) / m_RealToArray.y);
                int pos = ArrPos(xpos, ypos);
                double dvalue = m_Array[pos];
                //qDebug() << "Value = " << dvalue;
                return dvalue;
    }

    void setData(double * Array, int sizex, int sizey)
    {
            m_DataSize.x = sizex;
            m_DataSize.y = sizey;
            int size = sizex * sizey;
            setInterval( Qt::ZAxis, QwtInterval( 0.0,2.0));
            if (m_Array != NULL)
                    delete [] m_Array;
            m_Array = new double [size];
            memcpy(m_Array, Array, size * sizeof(double));
            qDebug() << "m_RangeX.min is " << m_RangeX.min << " m_RangeX.max is " << m_RangeX.max;
            qDebug() << "m_RangeY.min is " << m_RangeY.min << " m_RangeY.max is " << m_RangeY.max;
            qDebug() << "m_DataSize.x is " << m_DataSize.x << " m_DataSize.y is " << m_DataSize.y;
            m_RealToArray.x = (m_RangeX.max - m_RangeX.min) / (m_DataSize.x - 1);
            m_RealToArray.y = (m_RangeY.max - m_RangeY.min) / (m_DataSize.y - 1);
            qDebug() << "m_RealToArray.x is " << m_RealToArray.x << " m_RealToArray.y is " << m_RealToArray.y;
    }

    void setRangeX(const double min, const double max)
    {
            m_RangeX.min = min;
            m_RangeX.max = max;
            setInterval( Qt::XAxis, QwtInterval( min, max ) );
    }

    void setRangeY(const double min, const double max)
    {
            m_RangeY.min = min;
            m_RangeY.max = max;
            setInterval( Qt::YAxis, QwtInterval( min, max ) );
    }

    int ArrPos(const int x, const int y) const
    {
            return y + m_DataSize.y * x;
    }

};


class CPlotter : public QWidget //, protected Ui_CPlotter
{
        Q_OBJECT
public:
        CPlotter(QWidget* parent = 0, Qt::WFlags flags = 0);
        virtual ~CPlotter();


public:
    QHBoxLayout *hboxLayout;
    QwtPlot *qwtPlot;

    void setupUi(QWidget *widget)
    {
                QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                sizePolicy.setHorizontalStretch(20);
                sizePolicy.setVerticalStretch(20);
                sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
                widget->setSizePolicy(sizePolicy);
                widget->setAutoFillBackground(false);
                hboxLayout = new QHBoxLayout(widget);
                qwtPlot = new QwtPlot(widget);
                qwtPlot->setSizePolicy(sizePolicy);
                hboxLayout->addWidget(qwtPlot);

                QMetaObject::connectSlotsByName(widget);
    } // setupUi


private:
        QwtLinearColorMap *m_colorMap;
        QwtPlotSpectrogram * m_spectrogram;
        QwtScaleWidget * m_rightAxis;
        SpectrogramData * m_RasterData;
        QwtInterval m_DataRange;
        QwtPlotZoomer * zoomer;

public slots:
    void showContour(bool on);
    void showSpectrogram(bool on);


private:
        void initColorMap();
        void initColorBar();
        void initZoomer();

public:
        enum colormap{standard, gray, jet};

        void setData(double * Array, int sizex, int sizey);
        void setRangeX(const double min, const double max);
        void setRangeY(const double min, const double max);
        void setDataRange(double min, double max);
        void setDataAutoRange();
        void setColorMap(colormap map);


private:
        colormap m_colormap;
};

#endif // CPLOTTER_H
