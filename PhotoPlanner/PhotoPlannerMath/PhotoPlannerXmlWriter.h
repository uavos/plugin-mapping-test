#ifndef PHOTOPLANNERXMLWRITER_H
#define PHOTOPLANNERXMLWRITER_H

#include "PhotoPlannerCore.h"

#include <QDateTime>
#include <QXmlStreamWriter>
#include <QFile>

namespace aero_photo {

class PhotoPlannerXmlWriter {
public:
    PhotoPlannerXmlWriter(const FlightPoints &flightPoints, qreal velocity )
        : flightPoints_(flightPoints), velocity_(velocity) {
    }

    PhotoPlannerXmlWriter() = delete;
    PhotoPlannerXmlWriter(const PhotoPlannerXmlWriter &) = delete;
    PhotoPlannerXmlWriter(PhotoPlannerXmlWriter &&) = delete;

    void WritePackets(QString fileurlcvt, const int totalPointsInPacket) {
        fileurlcvt = fileurlcvt.remove(".xml");
        for(int i = 0; totalPointsInPacket * i < GetFlightPoints().size(); i++) {
            auto packname = QString("%1-%2.xml").arg(fileurlcvt).arg(i);
            if (i == 0) {
                packname = QString("%1.xml").arg(fileurlcvt);
            }
            QFile xmlFile(packname);
            if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QXmlStreamWriter stream(&xmlFile);
                stream.setAutoFormatting(true);
                stream.writeStartDocument();
                WritePacket(stream, i, totalPointsInPacket);
                stream.writeEndDocument();
            }
        }
    }

private:
    void WritePacket(QXmlStreamWriter &stream, int packetNumber, int totalPointsInPacket) {
        isSpeedWritten_ = false;

        stream.writeStartElement("mission");
        stream.writeAttribute("version", "9.10.56");
        stream.writeAttribute("href", "http://www.uavos.com/");
        stream.writeAttribute("title", "mission");
        stream.writeTextElement("hash", "f7c8603a8b1af004375ebc326a0ab6e");
        stream.writeTextElement("timestamp", QDateTime::currentDateTime().toString());
        if (packetNumber==0 && !GetFlightPoints().empty())
            WriteHome(stream, GetFlightPoints().front());
        WritePoints(stream, packetNumber, totalPointsInPacket);
        stream.writeEndElement();
    }
    void WriteHome(QXmlStreamWriter &stream, const GeoPoint &homePoint) {
        stream.writeStartElement("home");
        stream.writeTextElement("lat", QString::number(homePoint.latitude(), 'f', 15));
        stream.writeTextElement("lon", QString::number(homePoint.longitude(), 'f', 15));
        stream.writeTextElement("hmsl", "0");
        stream.writeEndElement();
    }
    void WritePoints(QXmlStreamWriter &stream, int packetNumber, int totalPointsInPacket) {
        stream.writeStartElement("waypoints");
        const int packetOffset = packetNumber * totalPointsInPacket;
        totalPointsInPacket = std::min(totalPointsInPacket, GetFlightPoints().size() - packetOffset);
        stream.writeAttribute("cnt", QString::number(totalPointsInPacket));
        const auto &points = GetFlightPoints();
        for (int index = 0; index < totalPointsInPacket ; index ++) {
            WriteOnePoint(stream, index, points[packetOffset + index]);
        }
        stream.writeEndElement();
    }
    void WriteOnePoint(QXmlStreamWriter &stream, int index, const FlightPoint &flightPoint) {
        // Conversion???
        stream.writeStartElement("waypoint");
        stream.writeAttribute("idx", QString::number(index));
        stream.writeTextElement("altitude", QString::number(flightPoint.altitude()));
        stream.writeTextElement("type", QString::number(flightPoint.type()));
        stream.writeTextElement("latitude", QString::number(flightPoint.latitude(), 'f', 15));
        stream.writeTextElement("longitude", QString::number(flightPoint.longitude(), 'f', 15));
        WriteOnePointActions(stream, flightPoint);
        stream.writeEndElement();
    }

    void WriteOnePointActions(QXmlStreamWriter &stream, const FlightPoint &flightPoint) {
        stream.writeStartElement("actions");
        if (!isSpeedWritten_) {
            stream.writeTextElement("speed", QString::number(velocity()));
            isSpeedWritten_ = true;
        }
        stream.writeTextElement("shot", (flightPoint.shotDistance()>0) ? "2" : "0" );
        stream.writeTextElement("dshot", QString::number(flightPoint.shotDistance()));
        stream.writeTextElement("POI", "0");
        stream.writeTextElement("loiter", "0");
        stream.writeTextElement("turnR", "0");
        stream.writeTextElement("loops", "0");
        stream.writeTextElement("time", "0");
        stream.writeEndElement();
    }


    const FlightPoints &GetFlightPoints() const  { return flightPoints_; }
    qreal velocity() const { return velocity_; }

    const FlightPoints &flightPoints_;
    const qreal velocity_;
    bool isSpeedWritten_ = false;
};

}

#endif // PHOTOPLANNER_H
