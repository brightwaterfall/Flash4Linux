#ifndef QT3TYPES_H
#define QT3TYPES_H

#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>
#include <QtGui/QPolygon>
#include <QtGui/QTransform>
#include <QtGui/QPainterPath>
#include <QtGui/QKeySequence>
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLayout>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QMdiSubWindow>
#include <QtWidgets/QWhatsThis>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtCore/QUrl>

class QFilePreview
{
public:
    virtual ~QFilePreview() {}
    virtual void previewUrl(const QUrl &) {}
};

inline QPixmap q3GrabWindow(WId id, int x, int y, int w, int h)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen)
        return QPixmap();
    return screen->grabWindow(id, x, y, w, h);
}

#ifndef TRUE
#define TRUE true
#endif
#ifndef FALSE
#define FALSE false
#endif

typedef Qt::WindowFlags WFlags;

#define WDestructiveClose 0x100000

inline void q3SetPaletteBackground(QWidget *w, const QColor &c)
{
    w->setAutoFillBackground(true);
    QPalette pal = w->palette();
    pal.setColor(w->backgroundRole(), c);
    pal.setColor(QPalette::Window, c);
    pal.setColor(QPalette::Base, c);
    w->setPalette(pal);
}

inline QColor q3BackgroundColor(const QWidget *w)
{
    return w->palette().color(w->backgroundRole());
}

inline void q3SetCaption(QWidget *w, const QString &title)
{
    w->setWindowTitle(title);
    QWidget *parent = w->parentWidget();
    if (parent && parent->inherits("QMdiSubWindow"))
        parent->setWindowTitle(title);
}

inline QAction *q3NewAction(const QString &text, const QIcon &icon, const QString &menuText,
                            const QKeySequence &accel, QObject *parent, const char *name = 0,
                            bool toggle = false)
{
    QAction *action = new QAction(icon, menuText.isEmpty() ? text : menuText, parent);
    if (!text.isEmpty())
        action->setIconText(text);
    if (!accel.isEmpty())
        action->setShortcut(accel);
    if (name)
        action->setObjectName(QString::fromLatin1(name));
    action->setCheckable(toggle);
    return action;
}

inline QAction *q3NewAction(const QString &text, const QString &menuText, const QKeySequence &accel,
                            QObject *parent, const char *name = 0, bool toggle = false)
{
    return q3NewAction(text, QIcon(), menuText, accel, parent, name, toggle);
}

inline void q3AddTo(QAction *action, QWidget *widget)
{
    if (QMenu *menu = qobject_cast<QMenu *>(widget))
        menu->addAction(action);
    else if (QToolBar *bar = qobject_cast<QToolBar *>(widget))
        bar->addAction(action);
    else if (QMenuBar *bar = qobject_cast<QMenuBar *>(widget))
        bar->addAction(action);
}

inline void q3AddTo(QActionGroup *group, QWidget *widget)
{
    if (!group)
        return;
    const QList<QAction *> actions = group->actions();
    for (int i = 0; i < actions.size(); ++i)
        q3AddTo(actions.at(i), widget);
}

inline QWidget *q3Named(QWidget *widget, const char *name)
{
    if (name && name[0])
        widget->setObjectName(QString::fromLatin1(name));
    return widget;
}

inline QWidget *q3Widget(QWidget *parent, const char *name)
{
    return q3Named(new QWidget(parent), name);
}

inline QLabel *q3Label(QWidget *parent, const char *name)
{
    return static_cast<QLabel *>(q3Named(new QLabel(parent), name));
}

inline QPushButton *q3PushButton(QWidget *parent, const char *name)
{
    return static_cast<QPushButton *>(q3Named(new QPushButton(parent), name));
}

inline QLineEdit *q3LineEdit(QWidget *parent, const char *name)
{
    return static_cast<QLineEdit *>(q3Named(new QLineEdit(parent), name));
}

inline QComboBox *q3ComboBox(bool editable, QWidget *parent, const char *name)
{
    QComboBox *box = new QComboBox(parent);
    box->setEditable(editable);
    q3Named(box, name);
    return box;
}

inline QToolButton *q3ToolButton(QWidget *parent, const char *name)
{
    return static_cast<QToolButton *>(q3Named(new QToolButton(parent), name));
}

inline QScrollBar *q3ScrollBar(QWidget *parent, const char *name)
{
    return static_cast<QScrollBar *>(q3Named(new QScrollBar(parent), name));
}

inline QSplitter *q3Splitter(QWidget *parent, const char *name)
{
    return static_cast<QSplitter *>(q3Named(new QSplitter(parent), name));
}

inline QVBoxLayout *q3VBoxOn(QWidget *parent, int margin, int spacing, const char *name)
{
    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->setContentsMargins(margin, margin, margin, margin);
    layout->setSpacing(spacing);
    if (name)
        layout->setObjectName(QString::fromLatin1(name));
    return layout;
}

inline QHBoxLayout *q3HBoxOn(QWidget *parent, int margin, int spacing, const char *name)
{
    QHBoxLayout *layout = new QHBoxLayout(parent);
    layout->setContentsMargins(margin, margin, margin, margin);
    layout->setSpacing(spacing);
    if (name)
        layout->setObjectName(QString::fromLatin1(name));
    return layout;
}

inline QVBoxLayout *q3VBoxIn(QBoxLayout *parent, int margin, int spacing, const char *name)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(margin, margin, margin, margin);
    layout->setSpacing(spacing);
    if (name)
        layout->setObjectName(QString::fromLatin1(name));
    parent->addLayout(layout);
    return layout;
}

inline QHBoxLayout *q3HBoxIn(QBoxLayout *parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    parent->addLayout(layout);
    return layout;
}

inline QVBoxLayout *q3VBoxIn(QBoxLayout *parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    parent->addLayout(layout);
    return layout;
}

inline void q3Tighten(QLayout *layout)
{
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
}

class QWMatrix : public QTransform
{
public:
    QWMatrix() {}
    QWMatrix(const QTransform &other) : QTransform(other) {}
};

class QIconSet : public QIcon
{
public:
    QIconSet() {}
    QIconSet(const QPixmap &pixmap) : QIcon(pixmap), m_pixmap(pixmap) {}
    QPixmap pixmap(QIcon::Mode mode = QIcon::Normal, QIcon::State state = QIcon::Off) const
    {
        if (!m_pixmap.isNull() && mode == QIcon::Normal && state == QIcon::Off)
            return m_pixmap;
        QSize size = m_pixmap.isNull() ? QSize(32, 32) : m_pixmap.size();
        return QIcon::pixmap(size, mode, state);
    }

private:
    QPixmap m_pixmap;
};

class QPointArray : public QPolygon
{
public:
    QPointArray() {}
    explicit QPointArray(int size) : QPolygon(size) {}
    QPointArray(const QPointArray &other) : QPolygon(other) {}
    QPointArray(const QPolygon &other) : QPolygon(other) {}
    QPointArray &operator=(const QPointArray &other)
    {
        QPolygon::operator=(other);
        return *this;
    }
    QPoint point(int index) const { return at(index); }
    QPointArray cubicBezier() const
    {
        QPointArray curve;
        if (size() != 4)
            return curve;
        QPainterPath path;
        path.moveTo(at(0));
        path.cubicTo(at(1), at(2), at(3));
        const QList<QPolygonF> polygons = path.toSubpathPolygons();
        for (int i = 0; i < polygons.size(); ++i) {
            const QPolygonF &poly = polygons.at(i);
            for (int j = 0; j < poly.size(); ++j)
                curve << poly.at(j).toPoint();
        }
        return curve;
    }
};

template <class T>
class QPtrList
{
public:
    QPtrList() : m_autoDelete(false), m_current(-1) {}
    ~QPtrList() { clear(); }
    void setAutoDelete(bool on) { m_autoDelete = on; }
    void append(T *item) { m_items.append(item); }
    void prepend(T *item) { m_items.prepend(item); }
    uint count() const { return (uint)m_items.count(); }
    bool isEmpty() const { return m_items.isEmpty(); }
    T *at(int index) const
    {
        if (index < 0 || index >= m_items.count())
            return 0;
        return m_items.at(index);
    }
    T *operator[](int index) const { return at(index); }
    T *first()
    {
        if (m_items.isEmpty()) {
            m_current = -1;
            return 0;
        }
        m_current = 0;
        return m_items.at(0);
    }
    T *next()
    {
        if (m_current + 1 >= m_items.count()) {
            m_current = m_items.count();
            return 0;
        }
        ++m_current;
        return m_items.at(m_current);
    }
    T *last()
    {
        if (m_items.isEmpty())
            return 0;
        m_current = m_items.count() - 1;
        return m_items.at(m_current);
    }
    T *current() const
    {
        if (m_current < 0 || m_current >= m_items.count())
            return 0;
        return m_items.at(m_current);
    }
    bool remove(T *item)
    {
        int index = m_items.indexOf(item);
        if (index < 0)
            return false;
        takeAt(index);
        return true;
    }
    bool remove(uint index)
    {
        if ((int)index >= m_items.count())
            return false;
        takeAt((int)index);
        return true;
    }
    bool removeLast()
    {
        if (m_items.isEmpty())
            return false;
        return remove((uint)m_items.count() - 1);
    }
    void clear()
    {
        while (!m_items.isEmpty())
            takeAt(0);
        m_current = -1;
    }
    int findRef(const T *item) const { return m_items.indexOf(const_cast<T *>(item)); }

private:
    void takeAt(int index)
    {
        T *item = m_items.at(index);
        m_items.removeAt(index);
        if (m_autoDelete)
            delete item;
        if (m_current >= m_items.count())
            m_current = m_items.count() - 1;
    }
    QList<T *> m_items;
    bool m_autoDelete;
    int m_current;
};

template <class T>
class QValueList : public QList<T>
{
public:
    QValueList() {}
    QValueList(const QList<T> &other) : QList<T>(other) {}
};

template <class T>
using QValueVector = QVector<T>;

class QAccel
{
public:
    static QKeySequence stringToKey(const QString &sequence) { return QKeySequence(sequence); }
};

#endif
