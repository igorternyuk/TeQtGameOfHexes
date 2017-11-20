#include "button.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QFont>
#include <QBrush>
#include <QDebug>

Button::Button(const QPointF &pos, const QString &caption, QGraphicsItem *parent):
   QObject(), QGraphicsRectItem(parent)
{
    this->setBrush(QBrush(Qt::darkCyan));
    this->setRect(0, 0, 100, 50);
    QGraphicsTextItem *text = new QGraphicsTextItem(caption, this);
    text->setFont(QFont("times", 24));
    auto dx = (this->boundingRect().width() - text->boundingRect().width()) / 2;
    auto dy = (this->boundingRect().height() - text->boundingRect().height()) / 2;
    text->setPos(dx, dy);
    this->setAcceptHoverEvents(true);
    this->setPos(pos);
}

Button::~Button()
{
    qDebug() << "Button destructor was called";
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton)
        emit leftClick();
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(QBrush(Qt::cyan));
    this->update();
    QGraphicsRectItem::hoverEnterEvent(event);

}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setBrush(QBrush(Qt::darkCyan));
    this->update();
    QGraphicsRectItem::hoverLeaveEvent(event);
}
