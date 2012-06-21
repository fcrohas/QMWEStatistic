#include "CPlotter.h"
//#include <QRectF>

/* --------------------------------------
 * Zoomer Class, used in CPlotter
 * --------------------------------------
*/
class MyZoomer: public QwtPlotZoomer
{
public:
    MyZoomer(QwtPlotCanvas* canvas): QwtPlotZoomer(canvas)
    {
        setTrackerMode(QwtPicker::AlwaysOn);
    }

protected:
    virtual QwtText trackerText( const QPoint& p ) const
    {
        QwtText t( QwtPlotPicker::trackerText( p ));

        QColor c(Qt::white);
        c.setAlpha(180);
        t.setBackgroundBrush( QBrush(c) );

        return t;
    }
};

class ColorMap: public QwtLinearColorMap
{
public:
    ColorMap():
        QwtLinearColorMap( Qt::darkCyan, Qt::red )
    {
        addColorStop( 0.1, Qt::cyan );
        addColorStop( 0.10, Qt::green );
        addColorStop( 0.35, Qt::yellow );
    }
};


/* --------------------------------------
 * CPlotter Class
 * --------------------------------------
*/

CPlotter::CPlotter(QWidget* parent /*= 0*/, Qt::WFlags flags /*= 0*/) : QWidget(parent, flags)
{
        setupUi(this);
        m_spectrogram = new QwtPlotSpectrogram();
        m_spectrogram->setRenderThreadCount( 0 );
        m_RasterData = new SpectrogramData();
        setColorMap(standard);
        initColorMap();
        m_spectrogram->attach(qwtPlot);
        qwtPlot->plotLayout()->setAlignCanvasToScales(true);
        initColorBar();
        zoomer = NULL;
}


CPlotter::~CPlotter()
{
}

void CPlotter::setData(double * Array, int sizex, int sizey)
{
        m_RasterData->setData(Array, sizex, sizey);
        m_spectrogram->setData(m_RasterData);
        m_DataRange = m_spectrogram->data()->interval(Qt::ZAxis);
        setDataAutoRange();
        initColorBar();
        initZoomer();
}

void CPlotter::setRangeX(const double min, const double max)
{
        m_RasterData->setRangeX(min, max);
        qwtPlot->setAxisScale(QwtPlot::xBottom, min, max);
}

void CPlotter::setRangeY(const double min, const double max)
{
        m_RasterData->setRangeY(min, max);
        qwtPlot->setAxisScale(QwtPlot::yLeft, min, max);
}


void CPlotter::showContour(bool on)
{
    m_spectrogram->setDisplayMode(QwtPlotSpectrogram::ContourMode, on);
}

void CPlotter::showSpectrogram(bool on)
{
    m_spectrogram->setDisplayMode(QwtPlotSpectrogram::ImageMode, on);
    m_spectrogram->setDefaultContourPen(on ? QPen() : QPen(Qt::NoPen));
}

void CPlotter::setColorMap(colormap map)
{
        m_colormap = map;
        initColorMap();
        initColorBar();
}

void CPlotter::initColorMap()
{
        switch (m_colormap)
        {
                case standard:
                        m_colorMap = new QwtLinearColorMap(Qt::darkCyan, Qt::red);
                        m_colorMap->addColorStop(0.1, Qt::cyan);
                        m_colorMap->addColorStop(0.6, Qt::green);
                        m_colorMap->addColorStop(0.95, Qt::yellow);
                        break;
                case gray:
                    m_colorMap = new QwtLinearColorMap(Qt::black, Qt::white);
                        break;
                case jet:
                        double pos;
                        m_colorMap = new QwtLinearColorMap(QColor(0,0,189), QColor(132,0,0));
                        pos = 1.0/13.0*1.0; m_colorMap->addColorStop(pos, QColor(0,0,255));
                        pos = 1.0/13.0*2.0; m_colorMap->addColorStop(pos, QColor(0,66,255));
                        pos = 1.0/13.0*3.0; m_colorMap->addColorStop(pos, QColor(0,132,255));
                        pos = 1.0/13.0*4.0; m_colorMap->addColorStop(pos, QColor(0,189,255));
                        pos = 1.0/13.0*5.0; m_colorMap->addColorStop(pos, QColor(0,255,255));
                        pos = 1.0/13.0*6.0; m_colorMap->addColorStop(pos, QColor(66,255,189));
                        pos = 1.0/13.0*7.0; m_colorMap->addColorStop(pos, QColor(132,255,132));
                        pos = 1.0/13.0*8.0; m_colorMap->addColorStop(pos, QColor(189,255,66));
                        pos = 1.0/13.0*9.0; m_colorMap->addColorStop(pos, QColor(255,255,0));
                        pos = 1.0/13.0*10.0; m_colorMap->addColorStop(pos, QColor(255,189,0));
                        pos = 1.0/13.0*12.0; m_colorMap->addColorStop(pos, QColor(255,66,0));
                        pos = 1.0/13.0*13.0; m_colorMap->addColorStop(pos, QColor(189,0,0));
                        break;
        }
        m_spectrogram->setColorMap(m_colorMap);
}

void CPlotter::setDataRange(double min, double max)
{
        m_DataRange = QwtInterval(min, max);
}

void CPlotter::setDataAutoRange()
{
        m_DataRange = m_spectrogram->data()->interval(Qt::ZAxis);
        qDebug() << "Data Auto Z Range" << m_DataRange;
        if (m_DataRange.minValue() == m_DataRange.maxValue())
                m_DataRange = QwtInterval(m_DataRange.minValue(), m_DataRange.minValue() + 1);
}

void CPlotter::initColorBar()
{
    m_rightAxis = qwtPlot->axisWidget(QwtPlot::yRight);
    m_rightAxis->setTitle(tr("Error"));
    m_rightAxis->setColorBarEnabled(true);
    m_rightAxis->setColorMap(m_DataRange, new ColorMap());

    qwtPlot->setAxisScale(QwtPlot::yRight,
        m_DataRange.minValue(),
        m_DataRange.maxValue() );
    qwtPlot->enableAxis(QwtPlot::yRight);

}

void CPlotter::initZoomer()
{
    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

        if (zoomer != NULL)
                delete zoomer;
        zoomer = new MyZoomer(qwtPlot->canvas());
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
        Qt::RightButton, Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
        Qt::RightButton);

    QwtPlotPanner *panner = new QwtPlotPanner(qwtPlot->canvas());
    panner->setAxisEnabled(QwtPlot::yRight, false);
    panner->setMouseButton(Qt::MidButton);

    // Avoid jumping when labels with more/less digits
    // appear/disappear when scrolling vertically

    const QFontMetrics fm(qwtPlot->axisWidget(QwtPlot::yLeft)->font());
    QwtScaleDraw *sd = qwtPlot->axisScaleDraw(QwtPlot::yLeft);
    sd->setMinimumExtent( fm.width("100.00") );

    const QColor c(Qt::darkBlue);
    zoomer->setRubberBandPen(c);
    zoomer->setTrackerPen(c);
}
