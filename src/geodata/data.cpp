#include <QFile>
#include <QFileInfo>
#include <QLineF>
#include <QDate>
#include "gpxparser.h"
#include "tcxparser.h"
#include "csvparser.h"
#include "kmlparser.h"
#include "fitparser.h"
#include "igcparser.h"
#include "nmeaparser.h"
#include "data.h"

static GPXParser gpx;
static TCXParser tcx;
static KMLParser kml;
static FITParser fit;
static CSVParser csv;
static IGCParser igc;
static NMEAParser nmea;

static QHash<QString, Parser*> parsers()
{
	QHash<QString, Parser*> hash;

        hash.insert("gpx", &gpx);
        hash.insert("tcx", &tcx);
        hash.insert("kml", &kml);
        hash.insert("fit", &fit);
        hash.insert("csv", &csv);
        hash.insert("igc", &igc);
        hash.insert("nmea", &nmea);

	return hash;
}


QHash<QString, Parser*> Data::_parsers = parsers();

Data::Data(QObject *parent)
    : QObject(parent)
    , _errorLine(0)
    , _trackDistance(0)
    , _trackTime(0)
    , _trackMovingTime(0)
    , _trackDateRange(QDate(), QDate())
    , _routeDistance(0)
{}

Data::~Data()
{
    for (int i = 0; i < _tracks.count(); i++) {
        delete _tracks.at(i);
    }
    for (int i = 0; i < _routes.count(); i++) {
		delete _routes.at(i);
    }
}

void Data::processData()
{
	for (int i = _tracks.count(); i < _trackData.count(); i++) {
		Track *track = new Track(_trackData[i]);
		_tracks.append(track);
		_trackDistance += track->distance();
		_trackTime += track->time();
		_trackMovingTime += track->movingTime();

		const QDate &date = track->date().date();
		if (_trackDateRange.first.isNull() || _trackDateRange.first > date)
			_trackDateRange.first = date;
		if (_trackDateRange.second.isNull() || _trackDateRange.second < date)
			_trackDateRange.second = date;
		emit addedTrack(*track);
	}

	for (int i = _routes.count(); i < _routeData.count(); i++) {
		Route *route = new Route(_routeData[i]);
		_routes.append(route);
		_routeDistance += route->distance();
		emit addedRoute(*route);
	}
}

bool Data::loadFile(const QString &fileName)
{
	QFile file(fileName);
	QFileInfo fi(fileName);

	_errorString.clear();
	_errorLine = 0;

	if (!file.open(QFile::ReadOnly)) {
		_errorString = qPrintable(file.errorString());
		return false;
	}

	QHash<QString, Parser*>::iterator it;;
	int wpCurrCount = _waypoints.count();

	if ((it = _parsers.find(fi.suffix().toLower())) != _parsers.end()) {
		if (it.value()->parse(&file, _trackData, _routeData, _waypoints)) {
			processData();
			for (int i = wpCurrCount; i < _waypoints.count(); i++) {
				emit addedWaypoint(_waypoints.at(i));
			}
			return true;
		}

		_errorLine = it.value()->errorLine();
		_errorString = it.value()->errorString();
	} else {
		for (it = _parsers.begin(); it != _parsers.end(); it++) {
			if (it.value()->parse(&file, _trackData, _routeData, _waypoints)) {
				processData();
				for (int i = wpCurrCount; i < _waypoints.count(); i++) {
					emit addedWaypoint(_waypoints.at(i));
				}
				return true;
			}
			file.reset();
		}

		qWarning("Error loading data file: %s:\n", qPrintable(fileName));
		for (it = _parsers.begin(); it != _parsers.end(); it++)
			qWarning("%s: line %d: %s\n", qPrintable(it.key()),
			         it.value()->errorLine(), qPrintable(it.value()->errorString()));

		_errorLine = 0;
		_errorString = "Unknown format";
	}

	return false;
}

void Data::clear()
{
	for (int i = 0; i < _tracks.count(); i++)
		delete _tracks.at(i);
	_tracks.clear();
	_trackData.clear();

	for (int i = 0; i < _routes.count(); i++)
		delete _routes.at(i);
	_routes.clear();
	_routeData.clear();

	_trackDistance = 0;
	_trackTime = 0;
	_trackMovingTime = 0;
	_trackDateRange = DateRange(QDate(), QDate());
	_routeDistance = 0;

	emit cleared();
}

QString Data::formats()
{
	return tr("Supported files (*.csv *.fit *.gpx *.igc *.kml *.nmea *.tcx)")
            + ";;" + tr("CSV files (*.csv)") + ";;" + tr("FIT files (*.fit)") + ";;"
            + tr("GPX files (*.gpx)") + ";;" + tr("IGC files (*.igc)") + ";;"
            + tr("KML files (*.kml)") + ";;" + tr("NMEA files (*.nmea)") + ";;"
	  + tr("TCX files (*.tcx)") + ";;" + tr("All files (*)");
}

QStringList Data::filter()
{
	QStringList filter;
	QHash<QString, Parser*>::iterator it;

	for (it = _parsers.begin(); it != _parsers.end(); it++)
		filter << QString("*.%1").arg(it.key());

	return filter;
}
