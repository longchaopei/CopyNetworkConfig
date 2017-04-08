#ifndef DATATYPE_H
#define DATATYPE_H

class QString;

typedef struct CountryInfo {
    QString countryName;
    QString zoningCode;
    QString ipAddr;
    QString netmask;
    QString gateWay;
} COUNTRY_INFO;

#endif // DATATYPE_H
