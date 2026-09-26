#ifndef QT3WIDGETS_H
#define QT3WIDGETS_H

#include "qt3types.h"

#include <algorithm>

#include <QtCore/QMap>
#include <QtCore/QChildEvent>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QDrag>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDropEvent>
#include <QtWidgets/QAbstractScrollArea>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

typedef QHeaderView QHeader;

class QCanvas;
class QCanvasView;
class QCanvasItem;
class QCanvasRectangle;
class QTable;
class QTableItem;
class QListView;
class QListViewItem;
class QPopupMenu;
class QDockWindow;
class QWorkspace;
class QScrollView;

class QCanvasItemList : public QList<QCanvasItem *>
{
public:
    typedef QList<QCanvasItem *>::iterator Iterator;
    typedef QList<QCanvasItem *>::const_iterator ConstIterator;
    void pop_back()
    {
        if (!isEmpty())
            removeLast();
    }
};

class QCanvasItem
{
public:
    enum RttiValues {
        Rtti_Item = 0,
        Rtti_Sprite = 1,
        Rtti_PolygonalItem = 2,
        Rtti_Text = 3,
        Rtti_Polygon = 4,
        Rtti_Rectangle = 5,
        Rtti_Ellipse = 6,
        Rtti_Line = 7,
        Rtti_Spline = 8
    };

    explicit QCanvasItem(QCanvas *canvas);
    virtual ~QCanvasItem();

    double x() const { return m_x; }
    double y() const { return m_y; }
    double z() const { return m_z; }
    virtual void moveBy(double dx, double dy);
    void move(double x, double y);
    void setX(double x) { move(x, y()); }
    void setY(double y) { move(x(), y); }
    void setZ(double z);

    void show() { setVisible(true); }
    void hide() { setVisible(false); }
    virtual void setVisible(bool visible);
    bool isVisible() const { return m_visible; }

    virtual int rtti() const { return Rtti_Item; }
    virtual QRect boundingRect() const = 0;
    virtual QCanvasItemList collisions(bool exact) const;

    QCanvas *canvas() const { return m_canvas; }
    virtual void setCanvas(QCanvas *canvas);
    virtual void draw(QPainter &painter) = 0;

protected:
    void changed();

    QCanvas *m_canvas;
    double m_x;
    double m_y;
    double m_z;
    bool m_visible;
};

class QCanvasPolygonalItem : public QCanvasItem
{
public:
    explicit QCanvasPolygonalItem(QCanvas *canvas);
    virtual ~QCanvasPolygonalItem();

    virtual void setPen(const QPen &pen);
    virtual void setBrush(const QBrush &brush);
    void setPen(const QColor &color) { setPen(QPen(color)); }
    void setBrush(const QColor &color) { setBrush(QBrush(color)); }
    QPen pen() const { return m_pen; }
    QBrush brush() const { return m_brush; }
    virtual QPointArray areaPoints() const = 0;
    QRect boundingRect() const;
    int rtti() const { return Rtti_PolygonalItem; }

    void draw(QPainter &painter);
protected:
    virtual void drawShape(QPainter &painter) = 0;
    QPen m_pen;
    QBrush m_brush;
};

class QCanvasRectangle : public QCanvasPolygonalItem
{
public:
    explicit QCanvasRectangle(QCanvas *canvas);
    QCanvasRectangle(const QRect &rect, QCanvas *canvas);
    QCanvasRectangle(int x, int y, int width, int height, QCanvas *canvas);
    int width() const { return m_width; }
    int height() const { return m_height; }
    void setSize(int width, int height);
    QSize size() const { return QSize(m_width, m_height); }
    QRect rect() const { return QRect(int(x()), int(y()), m_width, m_height).normalized(); }
    QPointArray areaPoints() const;
    int rtti() const { return Rtti_Rectangle; }

protected:
    void drawShape(QPainter &painter);
    int m_width;
    int m_height;
};

class QCanvasEllipse : public QCanvasPolygonalItem
{
public:
    explicit QCanvasEllipse(QCanvas *canvas);
    QCanvasEllipse(int width, int height, QCanvas *canvas);
    int width() const { return m_width; }
    int height() const { return m_height; }
    void setSize(int width, int height);
    void setAngles(int start, int length);
    int angleStart() const { return m_start; }
    int angleLength() const { return m_length; }
    QPointArray areaPoints() const;
    int rtti() const { return Rtti_Ellipse; }

protected:
    void drawShape(QPainter &painter);
    int m_width;
    int m_height;
    int m_start;
    int m_length;
};

class QCanvasLine : public QCanvasPolygonalItem
{
public:
    explicit QCanvasLine(QCanvas *canvas);
    void setPoints(int x1, int y1, int x2, int y2);
    QPoint startPoint() const { return QPoint(m_x1, m_y1); }
    QPoint endPoint() const { return QPoint(m_x2, m_y2); }
    int rtti() const { return Rtti_Line; }
    void moveBy(double dx, double dy);
    QPointArray areaPoints() const;

protected:
    void drawShape(QPainter &painter);
    int m_x1, m_y1, m_x2, m_y2;
};

class QCanvasPolygon : public QCanvasPolygonalItem
{
public:
    explicit QCanvasPolygon(QCanvas *canvas);
    void setPoints(const QPointArray &points);
    QPointArray points() const { return m_poly; }
    void moveBy(double dx, double dy);
    QPointArray areaPoints() const { return m_poly; }
    int rtti() const { return Rtti_Polygon; }

protected:
    void drawShape(QPainter &painter);
    QPointArray m_poly;
};

class QCanvasSpline : public QCanvasPolygon
{
public:
    explicit QCanvasSpline(QCanvas *canvas);
    void setControlPoints(const QPointArray &points, bool closed = true);
    QPointArray controlPoints() const { return m_control; }
    bool closed() const { return m_closed; }
    int rtti() const { return Rtti_Spline; }

private:
    QPointArray m_control;
    bool m_closed;
};

class QCanvasText : public QCanvasItem
{
public:
    explicit QCanvasText(QCanvas *canvas);
    QCanvasText(const QString &text, QCanvas *canvas);
    void setText(const QString &text);
    void setFont(const QFont &font);
    void setColor(const QColor &color);
    QString text() const { return m_text; }
    QFont font() const { return m_font; }
    QColor color() const { return m_color; }
    int textFlags() const { return m_flags; }
    void setTextFlags(int flags);
    QRect boundingRect() const;
    int rtti() const { return Rtti_Text; }
    void draw(QPainter &painter);

private:
    QString m_text;
    QFont m_font;
    QColor m_color;
    int m_flags;
};

class QCanvasPixmapArray
{
public:
    explicit QCanvasPixmapArray(const QString &fileName);
    QImage image() const { return m_image; }

private:
    QImage m_image;
};

class QCanvasSprite : public QCanvasItem
{
public:
    QCanvasSprite(QCanvasPixmapArray *array, QCanvas *canvas);
    ~QCanvasSprite();
    QImage *image() { return &m_image; }
    QRect boundingRect() const;
    int rtti() const { return Rtti_Sprite; }
    void draw(QPainter &painter);

private:
    QImage m_image;
};

class QCanvas : public QObject
{
public:
    QCanvas(QObject *parent = 0, const char *name = 0);
    QCanvas(int width, int height);
    ~QCanvas();

    void resize(int width, int height);
    int width() const { return m_width; }
    int height() const { return m_height; }
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const { return m_background; }
    void setDoubleBuffering(bool) {}
    void update();
    void setAllChanged() { update(); }
    void setChanged(const QRect &) { update(); }
    QCanvasItemList allItems() const { return m_items; }
    QCanvasItemList collisions(const QPoint &point) const;

    void addView(QCanvasView *view);
    void removeView(QCanvasView *view);
    void addItem(QCanvasItem *item);
    void takeItem(QCanvasItem *item);

private:
    int m_width;
    int m_height;
    QColor m_background;
    QCanvasItemList m_items;
    QList<QCanvasView *> m_views;
};

class QCanvasView : public QAbstractScrollArea
{
public:
    QCanvasView(QCanvas *canvas, QWidget *parent = 0, const char *name = 0, WFlags flags = Qt::WindowFlags());
    ~QCanvasView();

    QCanvas *canvas() const { return m_canvas; }
    void setCanvas(QCanvas *canvas);
    int contentsX() const;
    int contentsY() const;
    int contentsWidth() const;
    int contentsHeight() const;
    void scrollBy(int dx, int dy);
    QPoint contentsToViewport(const QPoint &point) const;
    QWMatrix worldMatrix() const { return m_matrix; }
    QWMatrix inverseWorldMatrix() const { return m_matrix.inverted(); }
    void setWorldMatrix(const QWMatrix &matrix);

protected:
    virtual void contentsMousePressEvent(QMouseEvent *) {}
    virtual void contentsMouseReleaseEvent(QMouseEvent *) {}
    virtual void contentsMouseMoveEvent(QMouseEvent *) {}
    virtual void contentsMouseDoubleClickEvent(QMouseEvent *) {}
    virtual void contentsDropEvent(QDropEvent *) {}
    bool viewportEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool event(QEvent *event);
    void drawContents(QPainter *painter);

private:
    QPoint toContents(const QPoint &viewportPos) const;
    void updateScrollBars();
    QMouseEvent mappedEvent(QMouseEvent *event) const;

    QCanvas *m_canvas;
    QWMatrix m_matrix;
};

class QScrollView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    QScrollView(QWidget *parent = 0, const char *name = 0, WFlags flags = Qt::WindowFlags());
    ~QScrollView();
    void addChild(QWidget *child, int x = 0, int y = 0);
    void setHScrollBarMode(Qt::ScrollBarPolicy mode);
    void setVScrollBarMode(Qt::ScrollBarPolicy mode);
    int contentsX() const;
    int contentsY() const;
    void scrollBy(int dx, int dy);
    QPoint contentsToViewport(const QPoint &point) const;

signals:
    void contentsMoving(int x, int y);

protected:
    void scrollContentsBy(int dx, int dy);
    void resizeEvent(QResizeEvent *event);

private:
    void placeChild();
    void applyBarMode(QScrollBar *bar, Qt::ScrollBarPolicy mode);
    QWidget *m_child;
    int m_childX;
    int m_childY;
};

class QColorGroup
{
public:
    QColorGroup() {}
    QColor background() const { return QColor(255, 255, 255); }
    QColor foreground() const { return QColor(0, 0, 0); }
};

class QTableItem : public QTableWidgetItem
{
public:
    enum EditType { Never, OnTyping, WhenCurrent, Always };
    QTableItem(QTable *table, EditType editType, const QString &text);
    virtual ~QTableItem();
    virtual void paint(QPainter *painter, const QColorGroup &group, const QRect &rect, bool selected);
    QTable *table() const { return m_table; }
    QString text() const { return m_text; }
    void setText(const QString &text);

private:
    QTable *m_table;
    EditType m_editType;
    QString m_text;
};

class QTableSelection
{
public:
    QTableSelection() : m_top(0), m_left(0), m_bottom(0), m_right(0) {}
    void init(int row, int column) { m_top = m_bottom = row; m_left = m_right = column; }
    void expandTo(int row, int column)
    {
        m_bottom = row;
        m_right = column;
    }
    int topRow() const { return qMin(m_top, m_bottom); }
    int bottomRow() const { return qMax(m_top, m_bottom); }
    int leftCol() const { return qMin(m_left, m_right); }
    int rightCol() const { return qMax(m_left, m_right); }

private:
    int m_top;
    int m_left;
    int m_bottom;
    int m_right;
};

class QTableDelegate : public QStyledItemDelegate
{
public:
    explicit QTableDelegate(QTable *table);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QTable *m_table;
};

class QTable : public QTableWidget
{
public:
    QTable(int rows, int columns, QWidget *parent = 0, const char *name = 0);
    ~QTable();
    void setReadOnly(bool readOnly);
    void setHScrollBarMode(Qt::ScrollBarPolicy mode);
    void setVScrollBarMode(Qt::ScrollBarPolicy mode);
    void setLeftMargin(int margin);
    void setTopMargin(int margin);
    void setNumRows(int rows) { setRowCount(rows); }
    void setNumCols(int columns) { setColumnCount(columns); }
    int numRows() const { return rowCount(); }
    int numCols() const { return columnCount(); }
    void scrollBy(int dx, int dy)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + dx);
        verticalScrollBar()->setValue(verticalScrollBar()->value() + dy);
    }
    void setItem(int row, int column, QTableItem *item);
    QTableItem *item(int row, int column) const;
    void updateCell(int row, int column);
    void clearSelection(bool) { QTableWidget::clearSelection(); }
    void addSelection(const QTableSelection &selection);

protected:
    virtual void contentsMousePressEvent(QMouseEvent *event);
    virtual void contentsMouseReleaseEvent(QMouseEvent *event);
    virtual void contentsMouseMoveEvent(QMouseEvent *event);
    virtual void clicked(int, int, int, const QPoint &) {}
    virtual void pressed(int, int, int, const QPoint &) {}
    virtual void contextMenuRequested(int, int, const QPoint &) {}
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

class QListViewItem : public QTreeWidgetItem
{
public:
    QListViewItem(QListView *parent, const QString &label1 = QString(), const QString &label2 = QString(),
                  const QString &label3 = QString(), const QString &label4 = QString(),
                  const QString &label5 = QString(), const QString &label6 = QString(),
                  const QString &label7 = QString(), const QString &label8 = QString());
    void setPixmap(int column, const QPixmap &pixmap);
    void setPixmap(int column, const QPixmap *pixmap);
    QListViewItem *nextSibling() const;
    QListViewItem *firstChild() const;
    int depth() const;
    void setExpandable(bool expandable);
    void setHeight(int height);
    int height() const { return m_height; }
    virtual void setup() {}

private:
    int m_height;
};

class QListView : public QTreeWidget
{
public:
    enum WidthMode { Manual, Maximum, MaximumVisible };
    enum ResizeMode { NoColumn, AllColumns, LastColumn };

    QListView(QWidget *parent = 0, const char *name = 0, WFlags flags = Qt::WindowFlags());
    int addColumn(const QString &label);
    int addColumn(const QIcon &icon, const QString &label);
    void setColumnWidthMode(int column, WidthMode mode);
    void setResizeMode(ResizeMode mode);
    void setAllColumnsShowFocus(bool enable);
    void setHScrollBarMode(Qt::ScrollBarPolicy mode);
    QListViewItem *itemAt(const QPoint &point) const;
    QListViewItem *firstChild() const;
    QListViewItem *selectedItem() const;
    void setSelected(QListViewItem *item, bool selected);
    QPoint contentsToViewport(const QPoint &point) const;
    int treeStepSize() const { return indentation(); }
    int itemMargin() const { return 1; }

protected:
    virtual void contentsMousePressEvent(QMouseEvent *) {}
    virtual void contentsMouseReleaseEvent(QMouseEvent *) {}
    virtual void contentsMouseMoveEvent(QMouseEvent *) {}
    virtual void contentsDragEnterEvent(QDragEnterEvent *) {}
    virtual void contentsDragMoveEvent(QDragMoveEvent *) {}
    virtual void contentsDragLeaveEvent(QDragLeaveEvent *) {}
    virtual void contentsDropEvent(QDropEvent *) {}
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QMouseEvent mappedEvent(QMouseEvent *event) const;
};

class QPopupMenu : public QMenu
{
public:
    explicit QPopupMenu(QWidget *parent = 0, const char *name = 0);
    ~QPopupMenu();
    int insertItem(const QString &text);
    int insertItem(const QString &text, QPopupMenu *popup);
    int insertItem(const QString &text, const QObject *receiver, const char *member,
                   const QKeySequence &accel = QKeySequence());
    int insertItem(const QString &text, const QObject *receiver, const char *member,
                   int accel, int id);
    int insertSeparator();
    void setItemEnabled(int id, bool enabled);
    void setItemChecked(int id, bool checked);
    void setItemParameter(int id, int parameter);
    void popup(const QPoint &pos);

private:
    int store(QAction *action);
    void invoke(int id, const QObject *receiver, const char *member);
    int m_nextId;
    QMap<int, QAction *> m_actions;
    QMap<int, int> m_parameters;
    QMap<int, QByteArray> m_slots;
    QMap<int, QObject *> m_receivers;
    QMap<int, bool> m_withInt;
};

class QDockArea : public QWidget
{
public:
    explicit QDockArea(QWidget *parent = 0) : QWidget(parent) {}
    void moveDockWindow(QDockWindow *, const QPoint &, const QRect &, bool) {}
};

class QDockWindow : public QDockWidget
{
public:
    enum Place { InDock, OutsideDock };
    QDockWindow(Place place, QWidget *parent, const char *name = 0);
    void setResizeEnabled(bool enabled);
    void setHorizontalStretchable(bool enabled);
    void setVerticalStretchable(bool enabled);
    void setCaption(const QString &caption) { setWindowTitle(caption); }
    QDockArea *area() const { return m_area; }

private:
    QDockArea *m_area;
};

class QWorkspace : public QMdiArea
{
public:
    explicit QWorkspace(QWidget *parent = 0, const char *name = 0);
    QMdiSubWindow *addWindow(QWidget *widget, int flags = 0);
    QWidget *activeWindow() const;
    QList<QWidget *> windowList() const;
};

class QVBox : public QFrame
{
public:
    explicit QVBox(QWidget *parent = 0, const char *name = 0, WFlags flags = Qt::WindowFlags());

protected:
    void childEvent(QChildEvent *event);

private:
    QVBoxLayout *m_layout;
};

class QButtonGroup : public QGroupBox
{
public:
    QButtonGroup(QWidget *parent, const char *name = 0);
    QButtonGroup(const QString &title, QWidget *parent, const char *name = 0);
    void setExclusive(bool exclusive) { m_exclusive = exclusive; }
    bool isExclusive() const { return m_exclusive; }
    void setLineWidth(int width) { setFlat(width <= 0); }

protected:
    void childEvent(QChildEvent *event);

private:
    void init(const char *name);
    bool m_exclusive;
};

#endif
