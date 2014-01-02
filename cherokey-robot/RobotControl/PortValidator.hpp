/* 
 * File:   PortValidator.hpp
 * Author: Ivan Gubochkin
 *
 * Created on January 3, 2014, 1:38 AM
 */

#ifndef PORTVALIDATOR_HPP
#define	PORTVALIDATOR_HPP

#include <QValidator>

class PortValidator : public QValidator
{
    Q_OBJECT
    
public:
    PortValidator(QObject *parent = 0);
    virtual ~PortValidator();
    
    virtual void fixup(QString &) const;
    virtual State validate(QString &input, int &) const;
};

#endif	/* PORTVALIDATOR_HPP */
