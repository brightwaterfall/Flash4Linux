#include "qt3widgets.h"

#include <QtWidgets/QWhatsThis>
#include <QtWidgets/QApplication>

static bool zLess(const QCanvasItem *left, const QCanvasItem *right)
{
    return left->z() < right->z();
}

static bool zGreater(const QCanvasItem *left, const QCanvasItem *right)
{
    return left->z() > right->z();
}

QCanvasItem::QCanvasItem(QCanvas *canvas)
    : m_canvas(0), m_x(0), m_y(0), m_z(0), m_visible(false)
{
    setCanvas(canvas);
}

QCanvasItem::~QCanvasItem()
{
    QCanvas *owner = m_canvas;
    m_canvas = 0;
    if (owner)
        owner->takeItem(this);
}

void QCanvasItem::moveBy(double dx, double dy)
{
    m_x += dx;
    m_y += dy;
    changed();
}

void QCanvasItem::move(double x, double y)
{
    m_x = x;
    m_y = y;
    changed();
}

void QCanvasItem::setZ(double z)
{
    m_z = z;
    changed();
}

void QCanvasItem::setVisible(bool visible)
{
    m_visible = visible;
    changed();
}

void QCanvasItem::setCanvas(QCanvas *canvas)
{
    if (m_canvas == canvas)
        return;
    if (m_canvas)
        m_canvas->takeItem(this);
    m_canvas = canvas;
    if (m_canvas)
        m_canvas->addItem(this);
}

QCanvasItemList QCanvasItem::collisions(bool) const
{
    QCanvasItemList hit;
    if (!m_canvas)
        return hit;
    const QRect mine = boundingRect();
    QCanvasItemList items = m_canvas->allItems();
    for (int i = 0; i < items.size(); ++i) {
        QCanvasItem *item = items.at(i);
        if (!item || item == this || !item->isVisible())
            continue;
        if (mine.intersects(item->boundingRect()))
            hit.append(item);
    }
    std::sort(hit.begin(), hit.end(), zGreater);
    return hit;
}

void QCanvasItem::changed()
{
    if (m_canvas)
        m_canvas->update();
}

QCanvasPolygonalItem::QCanvasPolygonalItem(QCanvas *canvas)
    : QCanvasItem(canvas), m_pen(Qt::black), m_brush(Qt::NoBrush)
{
}

QCanvasPolygonalItem::~QCanvasPolygonalItem() {}

void QCanvasPolygonalItem::setPen(const QPen &pen)
{
    m_pen = pen;
    changed();
}

void QCanvasPolygonalItem::setBrush(const QBrush &brush)
{
    m_brush = brush;
    changed();
}

QRect QCanvasPolygonalItem::boundingRect() const
{
    QRect rect = areaPoints().boundingRect();
    int pad = qMax(1, m_pen.width());
    return rect.adjusted(-pad, -pad, pad, pad);
}

void QCanvasPolygonalItem::draw(QPainter &painter)
{
    if (!isVisible())
        return;
    painter.setPen(m_pen);
    painter.setBrush(m_brush);
    drawShape(painter);
}

QCanvasRectangle::QCanvasRectangle(QCanvas *canvas)
    : QCanvasPolygonalItem(canvas), m_width(0), m_height(0)
{
}

QCanvasRectangle::QCanvasRectangle(const QRect &rect, QCanvas *canvas)
    : QCanvasPolygonalItem(canvas), m_width(rect.width()), m_height(rect.height())
{
    move(rect.x(), rect.y());
}

QCanvasRectangle::QCanvasRectangle(int x, int y, int width, int height, QCanvas *canvas)
    : QCanvasPolygonalItem(canvas), m_width(width), m_height(height)
{
    move(x, y);
}

void QCanvasRectangle::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
    changed();
}

QPointArray QCanvasRectangle::areaPoints() const
{
    QRect rect = QRect(int(x()), int(y()), m_width, m_height).normalized();
    QPointArray points(4);
    points[0] = rect.topLeft();
    points[1] = rect.topRight();
    points[2] = rect.bottomRight();
    points[3] = rect.bottomLeft();
    return points;
}

void QCanvasRectangle::drawShape(QPainter &painter)
{
    painter.drawRect(QRect(int(x()), int(y()), m_width, m_height).normalized());
}

QCanvasEllipse::QCanvasEllipse(QCanvas *canvas)
    : QCanvasPolygonalItem(canvas), m_width(0), m_height(0), m_start(0), m_length(360 * 16)
{
}

QCanvasEllipse::QCanvasEllipse(int width, int height, QCanvas *canvas)
    : QCanvasPolygonalItem(canvas), m_width(width), m_height(height), m_start(0), m_length(360 * 16)
{
}

void QCanvasEllipse::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
    changed();
}

void QCanvasEllipse::setAngles(int start, int length)
{
    m_start = start;
    m_length = length;
    changed();
}

QPointArray QCanvasEllipse::areaPoints() const
{
    QRect rect(int(x() - m_width / 2.0), int(y() - m_height / 2.0), m_width, m_height);
    QPointArray points(4);
    points[0] = rect.topLeft();
    points[1] = rect.topRight();
    points[2] = rect.bottomRight();
    points[3] = rect.bottomLeft();
    return points;
}

void QCanvasEllipse::drawShape(QPainter &painter)
{
    QRect rect(int(x() - m_width / 2.0), int(y() - m_height / 2.0), m_width, m_height);
    if (m_length >= 360 * 16)
        painter.drawEllipse(rect);
    else
        painter.drawPie(rect, m_start, m_length);
}

QCanvasLine::QCanvasLine(QCanvas *canvas)
    : QCanvasPolygonalItem(canvas), m_x1(0), m_y1(0), m_x2(0), m_y2(0)
{
}

void QCanvasLine::setPoints(int x1, int y1, int x2, int y2)
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
    changed();
}

void QCanvasLine::moveBy(double dx, double dy)
{
    m_x1 += (int)dx;
    m_y1 += (int)dy;
    m_x2 += (int)dx;
    m_y2 += (int)dy;
    QCanvasItem::moveBy(dx, dy);
}

QPointArray QCanvasLine::areaPoints() const
{
    QPointArray points(2);
    points[0] = QPoint(m_x1, m_y1);
    points[1] = QPoint(m_x2, m_y2);
    return points;
}

void QCanvasLine::drawShape(QPainter &painter)
{
    painter.drawLine(m_x1, m_y1, m_x2, m_y2);
}

QCanvasPolygon::QCanvasPolygon(QCanvas *canvas)
    : QCanvasPolygonalItem(canvas)
{
}

void QCanvasPolygon::setPoints(const QPointArray &points)
{
    m_poly = points;
    changed();
}

void QCanvasPolygon::moveBy(double dx, double dy)
{
    for (int i = 0; i < m_poly.size(); ++i) {
        m_poly[i].setX(m_poly[i].x() + (int)dx);
        m_poly[i].setY(m_poly[i].y() + (int)dy);
    }
    QCanvasItem::moveBy(dx, dy);
}

void QCanvasPolygon::drawShape(QPainter &painter)
{
    painter.drawPolygon(m_poly);
}

QCanvasSpline::QCanvasSpline(QCanvas *canvas)
    : QCanvasPolygon(canvas), m_closed(true)
{
}

void QCanvasSpline::setControlPoints(const QPointArray &points, bool closed)
{
    m_control = points;
    m_closed = closed;
    QPainterPath path;
    if (points.size() > 0)
        path.moveTo(points.at(0));
    for (int i = 1; i + 2 < points.size(); i += 3)
        path.cubicTo(points.at(i), points.at(i + 1), points.at(i + 2));
    if (closed)
        path.closeSubpath();
    setPoints(QPointArray(path.toFillPolygon().toPolygon()));
}

QCanvasText::QCanvasText(QCanvas *canvas)
    : QCanvasItem(canvas), m_font(QApplication::font()), m_color(Qt::black), m_flags(Qt::AlignLeft | Qt::AlignTop)
{
}

QCanvasText::QCanvasText(const QString &text, QCanvas *canvas)
    : QCanvasItem(canvas), m_text(text), m_font(QApplication::font()), m_color(Qt::black),
      m_flags(Qt::AlignLeft | Qt::AlignTop)
{
}

void QCanvasText::setText(const QString &text)
{
    m_text = text;
    changed();
}

void QCanvasText::setFont(const QFont &font)
{
    m_font = font;
    changed();
}

void QCanvasText::setColor(const QColor &color)
{
    m_color = color;
    changed();
}

void QCanvasText::setTextFlags(int flags)
{
    m_flags = flags;
    changed();
}

QRect QCanvasText::boundingRect() const
{
    QFontMetrics metrics(m_font);
    QRect rect = metrics.boundingRect(QRect(0, 0, 10000, 10000), m_flags, m_text);
    rect.moveTopLeft(QPoint(int(x()), int(y())));
    if (rect.width() < 1)
        rect.setWidth(1);
    if (rect.height() < 1)
        rect.setHeight(metrics.height());
    return rect;
}

void QCanvasText::draw(QPainter &painter)
{
    if (!isVisible())
        return;
    painter.setPen(m_color);
    painter.setFont(m_font);
    QRect rect = boundingRect();
    painter.drawText(rect, m_flags, m_text);
}

QCanvasPixmapArray::QCanvasPixmapArray(const QString &fileName)
    : m_image(fileName)
{
}

QCanvasSprite::QCanvasSprite(QCanvasPixmapArray *array, QCanvas *canvas)
    : QCanvasItem(canvas)
{
    if (array)
        m_image = array->image();
    delete array;
}

QCanvasSprite::~QCanvasSprite() {}

QRect QCanvasSprite::boundingRect() const
{
    return QRect(int(x()), int(y()), m_image.width(), m_image.height());
}

void QCanvasSprite::draw(QPainter &painter)
{
    if (!isVisible() || m_image.isNull())
        return;
    painter.drawImage(int(x()), int(y()), m_image);
}

QCanvas::QCanvas(QObject *parent, const char *name)
    : QObject(parent), m_width(0), m_height(0), m_background(Qt::white)
{
    if (name)
        setObjectName(QString::fromLatin1(name));
}

QCanvas::QCanvas(int width, int height)
    : QObject(0), m_width(width), m_height(height), m_background(Qt::white)
{
}

QCanvas::~QCanvas()
{
    QCanvasItemList copy = m_items;
    m_items.clear();
    for (int i = 0; i < copy.size(); ++i) {
        copy.at(i)->setCanvas(0);
        delete copy.at(i);
    }
}

void QCanvas::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    update();
}

void QCanvas::setBackgroundColor(const QColor &color)
{
    m_background = color;
    update();
}

void QCanvas::update()
{
    for (int i = 0; i < m_views.size(); ++i)
        m_views.at(i)->viewport()->update();
}

QCanvasItemList QCanvas::collisions(const QPoint &point) const
{
    QCanvasItemList hit;
    for (int i = 0; i < m_items.size(); ++i) {
        QCanvasItem *item = m_items.at(i);
        if (item->isVisible() && item->boundingRect().contains(point))
            hit.append(item);
    }
    std::sort(hit.begin(), hit.end(), zGreater);
    return hit;
}

void QCanvas::addView(QCanvasView *view)
{
    if (!m_views.contains(view))
        m_views.append(view);
}

void QCanvas::removeView(QCanvasView *view)
{
    m_views.removeAll(view);
}

void QCanvas::addItem(QCanvasItem *item)
{
    if (item && !m_items.contains(item))
        m_items.append(item);
}

void QCanvas::takeItem(QCanvasItem *item)
{
    m_items.removeAll(item);
}

QCanvasView::QCanvasView(QCanvas *canvas, QWidget *parent, const char *name, WFlags)
    : QAbstractScrollArea(parent), m_canvas(0)
{
    q3Named(this, name);
    viewport()->setMouseTracking(true);
    setMouseTracking(true);
    setCanvas(canvas);
}

QCanvasView::~QCanvasView()
{
    if (m_canvas)
        m_canvas->removeView(this);
}

void QCanvasView::setCanvas(QCanvas *canvas)
{
    if (m_canvas)
        m_canvas->removeView(this);
    m_canvas = canvas;
    if (m_canvas)
        m_canvas->addView(this);
    updateScrollBars();
    viewport()->update();
}

int QCanvasView::contentsX() const
{
    return horizontalScrollBar()->value();
}

int QCanvasView::contentsY() const
{
    return verticalScrollBar()->value();
}

int QCanvasView::contentsWidth() const
{
    return m_canvas ? m_canvas->width() : viewport()->width();
}

int QCanvasView::contentsHeight() const
{
    return m_canvas ? m_canvas->height() : viewport()->height();
}

void QCanvasView::scrollBy(int dx, int dy)
{
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + dx);
    verticalScrollBar()->setValue(verticalScrollBar()->value() + dy);
}

QPoint QCanvasView::contentsToViewport(const QPoint &point) const
{
    return QPoint(point.x() - contentsX(), point.y() - contentsY());
}

void QCanvasView::setWorldMatrix(const QWMatrix &matrix)
{
    m_matrix = matrix;
    viewport()->update();
}

QPoint QCanvasView::toContents(const QPoint &viewportPos) const
{
    return QPoint(viewportPos.x() + contentsX(), viewportPos.y() + contentsY());
}

QMouseEvent QCanvasView::mappedEvent(QMouseEvent *event) const
{
    return QMouseEvent(event->type(), QPointF(toContents(event->pos())), QPointF(event->globalPos()),
                       event->button(), event->buttons(), event->modifiers());
}

void QCanvasView::updateScrollBars()
{
    int width = m_canvas ? m_canvas->width() : 0;
    int height = m_canvas ? m_canvas->height() : 0;
    horizontalScrollBar()->setPageStep(qMax(1, viewport()->width()));
    verticalScrollBar()->setPageStep(qMax(1, viewport()->height()));
    horizontalScrollBar()->setRange(0, qMax(0, width - viewport()->width()));
    verticalScrollBar()->setRange(0, qMax(0, height - viewport()->height()));
}

void QCanvasView::drawContents(QPainter *painter)
{
    if (!m_canvas)
        return;
    painter->fillRect(QRect(contentsX(), contentsY(), viewport()->width(), viewport()->height()),
                      m_canvas->backgroundColor());
    painter->translate(-contentsX(), -contentsY());
    painter->setTransform(m_matrix, true);
    QCanvasItemList items = m_canvas->allItems();
    std::sort(items.begin(), items.end(), zLess);
    for (int i = 0; i < items.size(); ++i) {
        if (items.at(i)->isVisible())
            items.at(i)->draw(*painter);
    }
}

bool QCanvasView::viewportEvent(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Paint: {
        QPainter painter(viewport());
        drawContents(&painter);
        return true;
    }
    case QEvent::MouseButtonPress: {
        QMouseEvent mapped = mappedEvent(static_cast<QMouseEvent *>(event));
        contentsMousePressEvent(&mapped);
        return true;
    }
    case QEvent::MouseButtonRelease: {
        QMouseEvent mapped = mappedEvent(static_cast<QMouseEvent *>(event));
        contentsMouseReleaseEvent(&mapped);
        return true;
    }
    case QEvent::MouseMove: {
        QMouseEvent mapped = mappedEvent(static_cast<QMouseEvent *>(event));
        contentsMouseMoveEvent(&mapped);
        return true;
    }
    case QEvent::MouseButtonDblClick: {
        QMouseEvent mapped = mappedEvent(static_cast<QMouseEvent *>(event));
        contentsMouseDoubleClickEvent(&mapped);
        return true;
    }
    case QEvent::Enter:
        enterEvent(event);
        return false;
    case QEvent::Leave:
        leaveEvent(event);
        return false;
    default:
        break;
    }
    return QAbstractScrollArea::viewportEvent(event);
}

void QCanvasView::paintEvent(QPaintEvent *)
{
    viewport()->update();
}

void QCanvasView::resizeEvent(QResizeEvent *event)
{
    QAbstractScrollArea::resizeEvent(event);
    updateScrollBars();
}

bool QCanvasView::event(QEvent *event)
{
    bool result = QAbstractScrollArea::event(event);
    if (event->type() == QEvent::CursorChange && viewport())
        viewport()->setCursor(cursor());
    return result;
}

QScrollView::QScrollView(QWidget *parent, const char *name, WFlags)
    : QAbstractScrollArea(parent), m_child(0), m_childX(0), m_childY(0)
{
    q3Named(this, name);
}

QScrollView::~QScrollView() {}

void QScrollView::addChild(QWidget *child, int x, int y)
{
    m_child = child;
    m_childX = x;
    m_childY = y;
    if (m_child)
        m_child->setParent(viewport());
    placeChild();
}

void QScrollView::applyBarMode(QScrollBar *bar, Qt::ScrollBarPolicy mode)
{
    if (bar == horizontalScrollBar())
        setHorizontalScrollBarPolicy(mode);
    else
        setVerticalScrollBarPolicy(mode);
}

void QScrollView::setHScrollBarMode(Qt::ScrollBarPolicy mode)
{
    applyBarMode(horizontalScrollBar(), mode);
}

void QScrollView::setVScrollBarMode(Qt::ScrollBarPolicy mode)
{
    applyBarMode(verticalScrollBar(), mode);
}

int QScrollView::contentsX() const
{
    return horizontalScrollBar()->value();
}

int QScrollView::contentsY() const
{
    return verticalScrollBar()->value();
}

void QScrollView::scrollBy(int dx, int dy)
{
    horizontalScrollBar()->setValue(contentsX() + dx);
    verticalScrollBar()->setValue(contentsY() + dy);
    placeChild();
    emit contentsMoving(contentsX(), contentsY());
}

QPoint QScrollView::contentsToViewport(const QPoint &point) const
{
    return QPoint(point.x() - contentsX(), point.y() - contentsY());
}

void QScrollView::placeChild()
{
    if (!m_child)
        return;
    m_child->move(m_childX - contentsX(), m_childY - contentsY());
    int extraW = qMax(0, m_child->width() - viewport()->width());
    int extraH = qMax(0, m_child->height() - viewport()->height());
    horizontalScrollBar()->setRange(0, extraW);
    verticalScrollBar()->setRange(0, extraH);
    horizontalScrollBar()->setPageStep(qMax(1, viewport()->width()));
    verticalScrollBar()->setPageStep(qMax(1, viewport()->height()));
}

void QScrollView::scrollContentsBy(int dx, int dy)
{
    QAbstractScrollArea::scrollContentsBy(dx, dy);
    if (m_child)
        m_child->move(m_childX - contentsX(), m_childY - contentsY());
    emit contentsMoving(contentsX(), contentsY());
}

void QScrollView::resizeEvent(QResizeEvent *event)
{
    QAbstractScrollArea::resizeEvent(event);
    placeChild();
}

QTableItem::QTableItem(QTable *table, EditType editType, const QString &text)
    : QTableWidgetItem(text), m_table(table), m_editType(editType), m_text(text)
{
    setFlags(flags() & ~Qt::ItemIsEditable);
}

QTableItem::~QTableItem() {}

void QTableItem::setText(const QString &text)
{
    m_text = text;
    QTableWidgetItem::setText(text);
}

void QTableItem::paint(QPainter *painter, const QColorGroup &, const QRect &rect, bool selected)
{
    painter->fillRect(rect, selected ? QColor(10, 36, 106) : QColor(255, 255, 255));
    if (!m_text.isEmpty()) {
        painter->setPen(selected ? Qt::white : Qt::black);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_text);
    }
}

void QTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTableItem *cell = m_table->item(index.row(), index.column());
    painter->save();
    painter->translate(option.rect.topLeft());
    QRect local(0, 0, option.rect.width(), option.rect.height());
    bool selected = option.state & QStyle::State_Selected;
    if (cell)
        cell->paint(painter, QColorGroup(), local, selected);
    else
        painter->fillRect(local, QColor(255, 255, 255));
    painter->restore();
}

QTableDelegate::QTableDelegate(QTable *table)
    : QStyledItemDelegate(table), m_table(table)
{
}

QTable::QTable(int rows, int columns, QWidget *parent, const char *name)
    : QTableWidget(rows, columns, parent)
{
    q3Named(this, name);
    setItemDelegate(new QTableDelegate(this));
    setShowGrid(false);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    horizontalHeader()->hide();
    verticalHeader()->hide();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

QTable::~QTable() {}

void QTable::setReadOnly(bool readOnly)
{
    setEditTriggers(readOnly ? QAbstractItemView::NoEditTriggers : QAbstractItemView::DoubleClicked);
}

void QTable::setHScrollBarMode(Qt::ScrollBarPolicy mode)
{
    setHorizontalScrollBarPolicy(mode);
}

void QTable::setVScrollBarMode(Qt::ScrollBarPolicy mode)
{
    setVerticalScrollBarPolicy(mode);
}

void QTable::setLeftMargin(int margin)
{
    if (margin <= 0)
        verticalHeader()->hide();
    else
        verticalHeader()->setFixedWidth(margin);
}

void QTable::setTopMargin(int margin)
{
    if (margin <= 0)
        horizontalHeader()->hide();
    else
        horizontalHeader()->setFixedHeight(margin);
}

void QTable::setItem(int row, int column, QTableItem *item)
{
    QTableWidget::setItem(row, column, item);
}

QTableItem *QTable::item(int row, int column) const
{
    return static_cast<QTableItem *>(QTableWidget::item(row, column));
}

void QTable::updateCell(int row, int column)
{
    QModelIndex index = model()->index(row, column);
    if (index.isValid())
        update(index);
}

void QTable::addSelection(const QTableSelection &selection)
{
    setRangeSelected(QTableWidgetSelectionRange(selection.topRow(), selection.leftCol(),
                                                selection.bottomRow(), selection.rightCol()),
                     true);
}

void QTable::contentsMousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
}

void QTable::contentsMouseReleaseEvent(QMouseEvent *event)
{
    QTableWidget::mouseReleaseEvent(event);
}

void QTable::contentsMouseMoveEvent(QMouseEvent *event)
{
    QTableWidget::mouseMoveEvent(event);
}

void QTable::mousePressEvent(QMouseEvent *event)
{
    contentsMousePressEvent(event);
}

void QTable::mouseReleaseEvent(QMouseEvent *event)
{
    contentsMouseReleaseEvent(event);
}

void QTable::mouseMoveEvent(QMouseEvent *event)
{
    contentsMouseMoveEvent(event);
}

QListViewItem::QListViewItem(QListView *parent, const QString &label1, const QString &label2,
                             const QString &label3, const QString &label4, const QString &label5,
                             const QString &label6, const QString &label7, const QString &label8)
    : QTreeWidgetItem(parent), m_height(20)
{
    setText(0, label1);
    setText(1, label2);
    setText(2, label3);
    setText(3, label4);
    setText(4, label5);
    setText(5, label6);
    setText(6, label7);
    setText(7, label8);
    setSizeHint(0, QSize(1, m_height));
}

void QListViewItem::setPixmap(int column, const QPixmap &pixmap)
{
    setIcon(column, QIcon(pixmap));
}

void QListViewItem::setPixmap(int column, const QPixmap *pixmap)
{
    if (!pixmap || pixmap->isNull())
        setIcon(column, QIcon());
    else
        setIcon(column, QIcon(*pixmap));
}

QListViewItem *QListViewItem::nextSibling() const
{
    QTreeWidgetItem *container = parent() ? parent() : treeWidget()->invisibleRootItem();
    int index = container->indexOfChild(const_cast<QListViewItem *>(this));
    return static_cast<QListViewItem *>(container->child(index + 1));
}

QListViewItem *QListViewItem::firstChild() const
{
    return static_cast<QListViewItem *>(child(0));
}

int QListViewItem::depth() const
{
    int level = 0;
    for (QTreeWidgetItem *item = parent(); item; item = item->parent())
        ++level;
    return level;
}

void QListViewItem::setExpandable(bool expandable)
{
    setChildIndicatorPolicy(expandable ? ShowIndicator : DontShowIndicatorWhenChildless);
}

void QListViewItem::setHeight(int height)
{
    m_height = height;
    setSizeHint(0, QSize(1, height));
}

QListView::QListView(QWidget *parent, const char *name, WFlags)
    : QTreeWidget(parent)
{
    q3Named(this, name);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setAllColumnsShowFocus(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

int QListView::addColumn(const QString &label)
{
    int column = columnCount();
    setColumnCount(column + 1);
    headerItem()->setText(column, label);
    return column;
}

int QListView::addColumn(const QIcon &icon, const QString &label)
{
    int column = addColumn(label);
    headerItem()->setIcon(column, icon);
    return column;
}

void QListView::setColumnWidthMode(int column, WidthMode mode)
{
    QHeaderView::ResizeMode resizeMode = QHeaderView::Interactive;
    if (mode == Manual)
        resizeMode = QHeaderView::Fixed;
    else
        resizeMode = QHeaderView::ResizeToContents;
    header()->setSectionResizeMode(column, resizeMode);
}

void QListView::setResizeMode(ResizeMode mode)
{
    header()->setStretchLastSection(mode == LastColumn);
}

void QListView::setAllColumnsShowFocus(bool enable)
{
    QTreeView::setAllColumnsShowFocus(enable);
}

void QListView::setHScrollBarMode(Qt::ScrollBarPolicy mode)
{
    setHorizontalScrollBarPolicy(mode);
}

QListViewItem *QListView::itemAt(const QPoint &point) const
{
    return static_cast<QListViewItem *>(QTreeWidget::itemAt(point));
}

QListViewItem *QListView::firstChild() const
{
    return static_cast<QListViewItem *>(topLevelItem(0));
}

QListViewItem *QListView::selectedItem() const
{
    return static_cast<QListViewItem *>(currentItem());
}

void QListView::setSelected(QListViewItem *item, bool selected)
{
    if (!item)
        return;
    item->setSelected(selected);
    if (selected)
        setCurrentItem(item);
}

QPoint QListView::contentsToViewport(const QPoint &point) const
{
    return point;
}

QMouseEvent QListView::mappedEvent(QMouseEvent *event) const
{
    return QMouseEvent(event->type(), QPointF(event->pos()), QPointF(event->globalPos()), event->button(),
                       event->buttons(), event->modifiers());
}

void QListView::mousePressEvent(QMouseEvent *event)
{
    contentsMousePressEvent(event);
}

void QListView::mouseReleaseEvent(QMouseEvent *event)
{
    contentsMouseReleaseEvent(event);
}

void QListView::mouseMoveEvent(QMouseEvent *event)
{
    contentsMouseMoveEvent(event);
}

QPopupMenu::QPopupMenu(QWidget *parent, const char *name)
    : QMenu(parent), m_nextId(1)
{
    q3Named(this, name);
}

QPopupMenu::~QPopupMenu() {}

int QPopupMenu::store(QAction *action)
{
    int id = m_nextId++;
    m_actions.insert(id, action);
    m_parameters.insert(id, id);
    return id;
}

int QPopupMenu::insertItem(const QString &text)
{
    return store(addAction(text));
}

int QPopupMenu::insertItem(const QString &text, QPopupMenu *popup)
{
    if (popup)
        popup->setTitle(text);
    return store(addMenu(popup));
}

int QPopupMenu::insertItem(const QString &text, const QObject *receiver, const char *member,
                           const QKeySequence &accel)
{
    int id = insertItem(text);
    QAction *action = m_actions.value(id);
    if (!accel.isEmpty() && action)
        action->setShortcut(accel);
    QString signature = QString::fromLatin1(member ? member : "");
    if (!signature.isEmpty() && signature.at(0).isDigit())
        signature = signature.mid(1);
    bool withInt = signature.contains(QLatin1String("(int)"));
    int paren = signature.indexOf(QLatin1Char('('));
    if (paren >= 0)
        signature = signature.left(paren);
    m_slots.insert(id, signature.toLatin1());
    m_receivers.insert(id, const_cast<QObject *>(receiver));
    m_withInt.insert(id, withInt);
    if (action) {
        connect(action, &QAction::triggered, this, [this, id]() {
            QObject *target = m_receivers.value(id, 0);
            QByteArray slot = m_slots.value(id);
            if (!target || slot.isEmpty())
                return;
            if (slot == "whatsThis") {
                QWhatsThis::enterWhatsThisMode();
                return;
            }
            if (m_withInt.value(id))
                QMetaObject::invokeMethod(target, slot.constData(), Q_ARG(int, m_parameters.value(id, id)));
            else
                QMetaObject::invokeMethod(target, slot.constData());
        });
    }
    return id;
}

int QPopupMenu::insertItem(const QString &text, const QObject *receiver, const char *member,
                           int accel, int id)
{
    int stored = insertItem(text, receiver, member, QKeySequence(accel));
    if (id >= 0)
        setItemParameter(stored, id);
    return stored;
}

int QPopupMenu::insertSeparator()
{
    addSeparator();
    return m_nextId++;
}

void QPopupMenu::setItemEnabled(int id, bool enabled)
{
    if (QAction *action = m_actions.value(id, 0))
        action->setEnabled(enabled);
}

void QPopupMenu::setItemChecked(int id, bool checked)
{
    if (QAction *action = m_actions.value(id, 0)) {
        action->setCheckable(true);
        action->setChecked(checked);
    }
}

void QPopupMenu::setItemParameter(int id, int parameter)
{
    m_parameters.insert(id, parameter);
}

void QPopupMenu::popup(const QPoint &pos)
{
    QMenu::popup(pos);
}

QDockWindow::QDockWindow(Place, QWidget *parent, const char *name)
    : QDockWidget(parent), m_area(new QDockArea(parent))
{
    q3Named(this, name);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
}

void QDockWindow::setResizeEnabled(bool enabled)
{
    QSizePolicy policy = sizePolicy();
    policy.setHorizontalPolicy(enabled ? QSizePolicy::Expanding : QSizePolicy::Preferred);
    policy.setVerticalPolicy(enabled ? QSizePolicy::Expanding : QSizePolicy::Preferred);
    setSizePolicy(policy);
}

void QDockWindow::setHorizontalStretchable(bool enabled)
{
    QSizePolicy policy = sizePolicy();
    policy.setHorizontalPolicy(enabled ? QSizePolicy::Expanding : QSizePolicy::Preferred);
    setSizePolicy(policy);
}

void QDockWindow::setVerticalStretchable(bool enabled)
{
    QSizePolicy policy = sizePolicy();
    policy.setVerticalPolicy(enabled ? QSizePolicy::Expanding : QSizePolicy::Preferred);
    setSizePolicy(policy);
}

QWorkspace::QWorkspace(QWidget *parent, const char *name)
    : QMdiArea(parent)
{
    q3Named(this, name);
    setViewMode(QMdiArea::SubWindowView);
    setOption(QMdiArea::DontMaximizeSubWindowOnActivation, true);
}

QMdiSubWindow *QWorkspace::addWindow(QWidget *widget, int)
{
    return addSubWindow(widget);
}

QWidget *QWorkspace::activeWindow() const
{
    QMdiSubWindow *sub = activeSubWindow();
    return sub ? sub->widget() : 0;
}

QList<QWidget *> QWorkspace::windowList() const
{
    QList<QWidget *> widgets;
    const QList<QMdiSubWindow *> subs = subWindowList();
    for (int i = 0; i < subs.size(); ++i) {
        if (subs.at(i)->widget())
            widgets.append(subs.at(i)->widget());
    }
    return widgets;
}

QVBox::QVBox(QWidget *parent, const char *name, WFlags)
    : QFrame(parent), m_layout(new QVBoxLayout(this))
{
    q3Named(this, name);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
}

void QVBox::childEvent(QChildEvent *event)
{
    QFrame::childEvent(event);
    if (!event->added() || !m_layout)
        return;
    QWidget *widget = qobject_cast<QWidget *>(event->child());
    if (!widget || m_layout->indexOf(widget) >= 0)
        return;
    m_layout->addWidget(widget);
}

QButtonGroup::QButtonGroup(QWidget *parent, const char *name)
    : QGroupBox(parent), m_exclusive(false)
{
    init(name);
}

QButtonGroup::QButtonGroup(const QString &title, QWidget *parent, const char *name)
    : QGroupBox(title, parent), m_exclusive(false)
{
    init(name);
}

void QButtonGroup::init(const char *name)
{
    q3Named(this, name);
}

void QButtonGroup::childEvent(QChildEvent *event)
{
    QGroupBox::childEvent(event);
    if (!event->added())
        return;
    QAbstractButton *button = qobject_cast<QAbstractButton *>(event->child());
    if (!button)
        return;
    connect(button, &QAbstractButton::toggled, this, [this, button](bool on) {
        if (!m_exclusive || !on)
            return;
        const QObjectList kids = children();
        for (int i = 0; i < kids.size(); ++i) {
            QAbstractButton *other = qobject_cast<QAbstractButton *>(kids.at(i));
            if (other && other != button && other->isChecked())
                other->setChecked(false);
        }
    });
}
