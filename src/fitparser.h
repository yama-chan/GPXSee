#ifndef FITPARSER_H
#define FITPARSER_H

#include "parser.h"

class FITParser : public Parser
{
public:
	FITParser(QList<TrackData> &tracks, QList<RouteData> &routes,
	  QList<Waypoint> &waypoints);
	~FITParser();

	bool loadFile(QFile *file);
	QString errorString() const {return _errorString;}
	int errorLine() const {return 0;}

private:
	typedef struct {
		quint8 header_size;
		quint8 protocol_version;
		quint16 profile_version;
		quint32 data_size;
		quint32 magic;
	} FileHeader;

	typedef struct {
		quint8 id;
		quint8 size;
		quint8 type;
	} Field;

	typedef struct {
		quint8 endian;
		quint16 global_id;
		quint8 num_fields;
		Field *fields;
	} MessageDefinition;


	bool readData(char *data, size_t size);
	template<class T> bool readValue(T &val);
	bool skipValue(size_t size);

	bool parseHeader();
	bool parseRecord();
	bool parseDefinitionMessage(quint8 header);
	bool parseCompressedMessage(quint8 header);
	bool parseDataMessage(quint8 header);
	bool parseData(MessageDefinition *def, quint8 offset);
	bool readField(Field *f, quint32 &val);


	QIODevice *_device;
	QString _errorString;

	quint32 _len;
	quint8 _endian;
	quint32 _timestamp;
	MessageDefinition _defs[16];
};

#endif // FITPARSER_H
