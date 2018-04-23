#ifndef EXCEL_PARSER_H
#define EXCEL_PARSER_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QAxObject>
#include <QSharedPointer>
#include <QVariant>
#include <QList>
class excel_parser
{
    QList<QList<QVariant>> cast_qvariant_to_llqvariant(const QVariant &v)
    {
        QList<QList<QVariant>> res;
        QVariantList varRows = v.toList();
        if(varRows.isEmpty())
        {
            return res;
        }
        const int rowCount = varRows.size();
        QVariantList rowData;
        for(int i=0;i<rowCount;++i)
        {
            rowData = varRows[i].toList();
            res.push_back(rowData);
        }
        return res;
    }

public:
    excel_parser(){}
    QList<QList<QVariant>> readSheet(QAxObject * sheet)
    {
        QVariant var;
        QList<QList<QVariant>> r;
        if (sheet != NULL && !sheet->isNull())
        {
            QAxObject *usedRange = sheet->querySubObject("UsedRange");
            if (NULL == usedRange || usedRange->isNull())
            {
                return r;
            }
            var = usedRange->dynamicCall("Value");
            delete usedRange;
        }
        return cast_qvariant_to_llqvariant(var);
    }


    static excel_parser& instance()
    {
        static excel_parser s;
        return s;
    }
};


class excel_button :public QPushButton,excel_parser
{
    excel_parser p;
public:
    excel_button(QPushButton *parent=0):QPushButton(parent)
    {
        this->setText(QObject::tr("选择文件"));
    }
    //void mousePressEvent(QMouseEvent *e)
   /// {
    //    QPushButton::mousePressEvent(e);
    //    if (e->isAccepted())
    //    {
           //parser it
    //    }
   // }
};
#endif // EXCEL_PARSER_H
