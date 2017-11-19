#pragma once

#include <QObject>
#include <QGraphicsRectItem>

class Button : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit Button(const QPointF &pos, const QString &caption,
                    QGraphicsItem *parent = nullptr);
    virtual ~Button();
signals:
    void leftClick();
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;


};
