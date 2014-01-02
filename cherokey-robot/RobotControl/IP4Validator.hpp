/* 
 * File:   IP4Validator.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 3, 2014, 12:59 AM
 */

#ifndef IP4VALIDATOR_HPP
#define	IP4VALIDATOR_HPP

#include <QValidator>

class IP4Validator : public QValidator
{
    Q_OBJECT
    
public:
    IP4Validator(QObject *parent = 0);
    virtual ~IP4Validator();
    
    virtual void fixup(QString &) const;
    virtual State validate(QString &input, int &) const;
};

#endif	/* IP4VALIDATOR_HPP */

