#include "Globals.hpp"

#include<QNetworkInterface>

quint32 getHostAddress()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    
    for (int i = 0; i < list.size(); ++i) 
    {
        QNetworkInterface inter = list.at(i);
        QNetworkInterface::InterfaceFlags flags = inter.flags();
        if ((flags & QNetworkInterface::IsUp) &&
            !(flags & QNetworkInterface::IsLoopBack))
        {
            QList<QNetworkAddressEntry> addresses = inter.addressEntries();
            if (!addresses.empty())
            {
                return addresses.at(0).ip().toIPv4Address();
            }
        }
    }
    
    return 0;
}
