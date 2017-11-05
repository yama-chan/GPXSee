#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QString>
#include <QList>
#include <QDate>
#include <QPrinter>
#include "units.h"
#include "timetype.h"
#include "graph.h"
#include "poi.h"
#include "exportdialog.h"
#include "optionsdialog.h"
#include "data.h"
#include "datalistview.h"

class QMenu;
class QToolBar;
class QTabWidget;
class QActionGroup;
class QAction;
class QLabel;
class QSignalMapper;
class QPrinter;
class FileBrowser;
class GraphTab;
class PathView;
class Map;
class MapList;

class GUI : public QMainWindow
{
	Q_OBJECT

public:
	GUI();
	~GUI();

	bool openFile(const QString &fileName);

private slots:
	void about();
	void keys();
	void dataSources();
	void printFile();
	void exportFile();
	void exportImageFile();
	void openFile();
	void closeAll();
	void reloadFile();
	void openPOIFile();
	void closePOIFiles();
	void showGraphs(bool show);
	void showGraphGrids(bool show);
	void showGraphSliderInfo(bool show);
	void showToolbars(bool show);
	void showFullscreen(bool show);
	void showTracks(bool show);
	void showRoutes(bool show);
	void loadMap();
	void nextMap();
	void prevMap();
	void openOptions();

	void mapChanged(int);
	void graphChanged(int);
	void poiFileChecked(int);

	void next();
	void prev();
	void last();
	void first();

	void setTotalTime() {setTimeType(Total);}
	void setMovingTime() {setTimeType(Moving);}
	void setMetricUnits() {setUnits(Metric);}
	void setImperialUnits() {setUnits(Imperial);}
	void setDistanceGraph() {setGraphType(Distance);}
	void setTimeGraph() {setGraphType(Time);}

	void sliderPositionChanged(qreal pos);

private:

	void loadDatums();
	void loadMaps();
	void loadPOIs();
	void closeFiles();
	void plot(QPrinter *printer);

	QAction *createPOIFileAction(int index);
	void createPOIFilesActions();
	void createMapActions();
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createDataListView();
	void createPathView();
	void createGraphTabs();
	void createBrowser();

	bool openPOIFile(const QString &fileName);
	bool loadFile(const QString &fileName);
	void exportFile(const QString &fileName);
	void updateStatusBarInfo();
	void updateWindowTitle();
	void updateNavigationActions();
	void updateGraphTabs();
	void updateDataListView();
	void updatePathView();

	TimeType timeType() const;
	Units units() const;
	void setTimeType(TimeType type);
	void setUnits(Units units);
	void setGraphType(GraphType type);

	qreal distance() const;
	qreal time() const;
	qreal movingTime() const;
	int mapIndex(const QString &name);
	void readSettings();
	void writeSettings();

	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	QToolBar *_fileToolBar;
	QToolBar *_showToolBar;
	QToolBar *_navigationToolBar;
	QMenu *_poiFilesMenu;
	QMenu *_mapMenu;

	QActionGroup *_fileActionGroup;
	QActionGroup *_navigationActionGroup;
	QActionGroup *_mapsActionGroup;
	QAction *_exitAction;
	QAction *_keysAction;
	QAction *_dataSourcesAction;
	QAction *_aboutAction;
	QAction *_aboutQtAction;
	QAction *_printFileAction;
	QAction *_exportFileAction;
	QAction *_exportImageFileAction;
	QAction *_openFileAction;
	QAction *_closeFileAction;
	QAction *_reloadFileAction;
	QAction *_openPOIAction;
	QAction *_closePOIAction;
	QAction *_showPOIAction;
	QAction *_overlapPOIAction;
	QAction *_showPOILabelsAction;
	QAction *_showMapAction;
	QAction *_fullscreenAction;
	QAction *_loadMapAction;
	QAction *_clearMapCacheAction;
	QAction *_showGraphsAction;
	QAction *_showGraphGridAction;
	QAction *_showGraphSliderInfoAction;
	QAction *_distanceGraphAction;
	QAction *_timeGraphAction;
	QAction *_showToolbarsAction;
	QAction *_nextAction;
	QAction *_prevAction;
	QAction *_lastAction;
	QAction *_firstAction;
	QAction *_metricUnitsAction;
	QAction *_imperialUnitsAction;
	QAction *_totalTimeAction;
	QAction *_movingTimeAction;
	QAction *_nextMapAction;
	QAction *_prevMapAction;
	QAction *_showTracksAction;
	QAction *_showRoutesAction;
	QAction *_showWaypointsAction;
	QAction *_showWaypointLabelsAction;
	QAction *_showRouteWaypointsAction;
	QAction *_openOptionsAction;
	QAction *_mapsEnd;
	QList<QAction*> _mapActions;
	QList<QAction*> _poiFilesActions;

	QSignalMapper *_poiFilesSignalMapper;
	QSignalMapper *_mapsSignalMapper;

	QLabel *_fileNameLabel;
	QLabel *_distanceLabel;
	QLabel *_timeLabel;

	PathView *_pathView;
	QTabWidget *_graphTabWidget;
	QList<GraphTab*> _tabs;

	POI *_poi;
	MapList *_ml;

	FileBrowser *_browser;
	QList<QString> _files;

	Map *_map;
	Data _data;
	QString _pathName;

	qreal _sliderPos;

	int _frameStyle;
	bool _showGraphs;

	Export _export;
	Options _options;
	DataListView *_dataListView;
};

#endif // GUI_H
