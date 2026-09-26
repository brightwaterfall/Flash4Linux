/****************************************************************************
** Form implementation generated from reading ui file 'tools.ui'
**
** Created: Mon Jun 9 06:20:51 2003
**      by:  ozkanpakdil@users.sourceforge.net
**
****************************************************************************/

#include "f4lm.h"
#include "tools.h"
#include "colorpopup.h"

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qwhatsthis.h>
#include <qlabel.h>
#include <qcolordialog.h>

#include "cursor/arrow_tool.xpm"
#include "cursor/lasso_tool.xpm"
#include "cursor/sub_selection_tool.xpm"
#include "cursor/text_tool.xpm"
#include "cursor/rectangle_tool.xpm"
#include "cursor/paint_bucket_tool.xpm"
#include "cursor/pencil_tool.xpm"
#include "cursor/ink_bottle_tool.xpm"
#include "cursor/oval_tool.xpm"
#include "cursor/pen_tool.xpm"
#include "cursor/line_tool.xpm"
#include "cursor/brush_tool.xpm"
#include "cursor/free_transform_tool.xpm"
#include "cursor/fill_transform_tool.xpm"
#include "cursor/eye_dropper_tool.xpm"
#include "cursor/eraser_tool.xpm"
#include "cursor/hand_tool.xpm"
#include "cursor/zoom_tool.xpm"
#include "./cursor/colorComboBoxButton.xpm"

CTools::CTools (QWidget * parent, const char *name, WFlags fl,
        F4lmApp * realp)
        :
        QWidget(parent)
{
    if (!name)
        setObjectName ("Tools");
    resize (600, 480);
    q3SetCaption(this, tr ("Tools"));
    dad = (F4lmApp *) realp;
    QVBoxLayout * topLayout = q3VBoxOn(this, 0, 2, 0);
        //QVBoxLayout *layer1=new QVBoxLayout(this);
    ToolsButtonGroup = new QButtonGroup (this, "ToolsButtonGroup");
        //ToolsButtonGroup->setGeometry( QRect( 150, 70, 71, 291 ) );
    ToolsButtonGroup->setLineWidth (2);
    ToolsButtonGroup->setTitle (tr (""));
    ToolsButtonGroup->setExclusive (true);

        //      DeleteLayer->setIconSet(QIconSet( QPixmap (( const char** ) delete_layer_xpm)));
        //  DeleteLayer->setTextLabel( trUtf8( "Delete Layer" ) );

    QVBoxLayout * layer1 = q3VBoxOn(ToolsButtonGroup, 0, 2, 0);
    QLabel * toolsLabel = q3Label(ToolsButtonGroup, "Tools");
    toolsLabel->setText (tr ("   Tools"));
    layer1->addWidget (toolsLabel);
    QHBoxLayout * layer2 = q3HBoxIn(layer1);
    ArrowTool =new CToolButton (ToolsButtonGroup,tr("Use the Arrow to select  drag and reshape the drawing"));
    ArrowTool->setIcon (QIconSet (QPixmap ((const char **) arrow_tool_xpm)));
    ArrowTool->setToolTip (tr ("Arrow Tool"));
    SubSelectionTool =new CToolButton (ToolsButtonGroup,tr("Use the Subselect tool to select  drag and reshape the drawing using handles"));
    SubSelectionTool->setIcon (QIconSet (QPixmap ((const char **) sub_selection_tool_xpm)));
    SubSelectionTool->setToolTip (tr ("Subselection Tool"));
    layer2->addWidget (ArrowTool);
    layer2->addWidget (SubSelectionTool);

    QHBoxLayout * layer3 = q3HBoxIn(layer1);
    LineTool =new CToolButton (ToolsButtonGroup,tr ("Use the Line tool  to draw lines"));
    LineTool->setIcon (QIconSet (QPixmap ((const char **) line_tool_xpm)));
    LineTool->setToolTip (tr ("Line Tool"));
    LassoTool =new CToolButton (ToolsButtonGroup,tr ("Use the Lasso to select areas of the drawing"));
    LassoTool->setIcon (QIconSet (QPixmap ((const char **) lasso_tool_xpm)));
    LassoTool->setToolTip (tr ("Lasso Tool"));
	
    layer3->addWidget (LineTool);
    layer3->addWidget (LassoTool);

    QHBoxLayout * layer4 = q3HBoxIn(layer1);
    PenTool =new CToolButton (ToolsButtonGroup,tr ("Use the Pen tool to draw lines and curves"));
    PenTool->setIcon (QIconSet (QPixmap ((const char **) pen_tool_xpm)));
    PenTool->setToolTip (tr ("Pen Tool"));
    TextTool =new CToolButton (ToolsButtonGroup,tr("Use the Text tool to create and edit formated text"));
    TextTool->setIcon (QIconSet (QPixmap ((const char **) text_tool_xpm)));
    TextTool->setToolTip (tr ("Text Tool"));
    layer4->addWidget (PenTool);
    layer4->addWidget (TextTool);

    QHBoxLayout * layer5 = q3HBoxIn(layer1);
    OvalTool =new CToolButton (ToolsButtonGroup,tr ("Use the Oval  tool  to draw oval shapes"));
    OvalTool->setIcon (QIconSet (QPixmap ((const char **) oval_tool_xpm)));
    OvalTool->setToolTip (tr ("Oval Tool"));
    RectangleTool =new CToolButton (ToolsButtonGroup,tr("Use the Rectangle tool to draw rectangles and rounded rectangles"));
    RectangleTool->setIcon (QIconSet (QPixmap ((const char **) rectangle_tool_xpm)));
    RectangleTool->setToolTip (tr ("Rectangle Tool"));
    layer5->addWidget (OvalTool);
    layer5->addWidget (RectangleTool);

    QHBoxLayout * layer6 = q3HBoxIn(layer1);
    PencilTool =new CToolButton (ToolsButtonGroup,tr ("Use the Pencil to draw lines and shapes"));
    PencilTool->setIcon (QIconSet (QPixmap ((const char **) pencil_tool_xpm)));
    PencilTool->setToolTip (tr ("Pencil Tool"));
    BrushTool =new CToolButton (ToolsButtonGroup,tr ("Use the Brush to paint filled areas"));
    BrushTool->setIcon (QIconSet (QPixmap ((const char **) brush_tool_xpm)));
    BrushTool->setToolTip (tr ("Brush Tool"));
    layer6->addWidget (PencilTool);
    layer6->addWidget (BrushTool);

    QHBoxLayout * layer7 = q3HBoxIn(layer1);

    FreeTransformTool =new CToolButton (ToolsButtonGroup,tr("Use Free Transform to select  drag and reshape the drawing"));

    FreeTransformTool->setIcon (QIconSet (QPixmap ((const char **) free_transform_tool_xpm)));
    FreeTransformTool->setToolTip (tr ("Free Transform Tool"));
    FillTransformTool =new CToolButton (ToolsButtonGroup,tr("Fill Transform shows handles to adjust the angle  position and size of a gradient or bitmap fill"));

    FillTransformTool->setIcon (QIconSet (QPixmap ((const char **) fill_transform_tool_xpm)));
    FillTransformTool->setToolTip (tr ("Fill Transform Tool"));
	
    layer7->addWidget (FreeTransformTool);
    layer7->addWidget (FillTransformTool);

        /////////////////////tansformatin tools waiting for future works :)///////////////////////
    FreeTransformTool->hide ();
    FillTransformTool->hide ();

    QHBoxLayout * layer8 = q3HBoxIn(layer1);
    InkBottleTool =new CToolButton (ToolsButtonGroup,tr("Use the Ink Bottle to apply line color and thickness to the drawing"));

    InkBottleTool->setIcon (QIconSet (QPixmap ((const char **) ink_bottle_tool_xpm)));
    InkBottleTool->setToolTip (tr ("Ink Bottle Tool"));
    PaintBucketTool =new CToolButton (ToolsButtonGroup,tr("Use the Paint Bucket to fill enclosed areas of the drawing with color"));

    PaintBucketTool->setIcon (QIconSet (QPixmap ((const char **) paint_bucket_tool_xpm)));
    PaintBucketTool->setToolTip (tr ("Paint Bucket Tool"));
    layer8->addWidget (InkBottleTool);
    layer8->addWidget (PaintBucketTool);

    QHBoxLayout * layer9 = q3HBoxIn(layer1);
    EyedropperTool =new CToolButton (ToolsButtonGroup,tr("Use the Dropper to pick up line  fill and text styles from the drawing"));

    EyedropperTool->setIcon (QIconSet (QPixmap ((const char **) eye_dropper_tool_xpm)));
    EyedropperTool->setToolTip (tr ("Eye Dropper Tool"));
    EraserTool =new CToolButton (ToolsButtonGroup,tr("Use the Eraser to erase lines and fills in the drawing"));
    EraserTool->setIcon (QIconSet (QPixmap ((const char **) eraser_tool_xpm)));
    EraserTool->setToolTip (tr ("Eraser Tool"));
    layer9->addWidget (EyedropperTool);
    layer9->addWidget (EraserTool);

        //ToolsViewGroup = new QButtonGroup( this, "ToolsButtonGroup" );
        //ToolsViewGroup->setExclusive(true);
    QLabel * viewLabel = q3Label(ToolsButtonGroup, "View");
    viewLabel->setText (tr ("   View"));
    QVBoxLayout * layer10 = q3VBoxIn(layer1, 0, 2, 0);
    layer10->addWidget (viewLabel);
    HandTool =new CToolButton (ToolsButtonGroup,tr ("Use the Hand  to move the view of the drawing"));
    HandTool->setIcon (QIconSet (QPixmap ((const char **) hand_tool_xpm)));
    HandTool->setToolTip (tr ("Hand Tool"));
	
    ZoomTool =new CToolButton (ToolsButtonGroup,tr("Use the Magnifer to enlarge or reduce the view of the drawing"));
    ZoomTool->setIcon (QIconSet (QPixmap ((const char **) zoom_tool_xpm)));
    ZoomTool->setToolTip (tr ("Zoom Tool"));
	
    QHBoxLayout * layer11 = q3HBoxIn(layer10);
    layer11->addWidget (HandTool);
    layer11->addWidget (ZoomTool);

    ToolsColorsGroup = new QButtonGroup (this, "ToolsColorsGroup");
    ToolsColorsGroup->setExclusive (true);
    QLabel * colorsLabel = q3Label(ToolsColorsGroup, "Colors");
    colorsLabel->setText ("   Colors");
    QVBoxLayout * layer12 = q3VBoxOn(ToolsColorsGroup, 0, 2, 0);
    layer12->addWidget (colorsLabel);
    StrokeColor = new CToolButton (ToolsColorsGroup);
    /*strokpix.resize(16,16);
       strokpix.load("./cursor/colorComboBoxButton.xpm");
       strokpix.fill(QColor(0,0,0)); */
    QImage * str = new QImage ((const char **) colorComboBoxButton);
    strokpix = QPixmap::fromImage(*str);
    fillpix = QPixmap(16, 16);
    fillpix.fill (QColor (0, 0, 0));
    StrokeColor->setIcon (QIconSet (strokpix));
    StrokeColor->setToolTip (tr ("Stroke Color"));
    FillColor = new CToolButton (ToolsColorsGroup);
    FillColor->setIcon (QIconSet (fillpix));
    FillColor->setToolTip (tr ("Fill Color"));
        //QHBoxLayout *layer13=new QHBoxLayout(layer12);
    layer12->addWidget (StrokeColor);
    layer12->addWidget (FillColor);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////////burdaki buttonlar henüz iþlenmemiþtir. kafadan yazýldý o kadar.//////////////////////////////////////////////////////
    QButtonGroup * ToolsOptionsGroup =new QButtonGroup (this, "ToolsOptionsGroup");
    ToolsOptionsGroup->setExclusive (true);
    QLabel * optionsLabel = q3Label(ToolsOptionsGroup, "Colors");
    optionsLabel->setText (tr ("   Options"));
    QVBoxLayout * layer13 = q3VBoxOn(ToolsOptionsGroup, 0, 2, 0);
    layer13->addWidget (optionsLabel);
    /*    QToolButton *_StrokeColor= new CToolButton( ToolsOptionsGroup);
    	strokpix.resize(16,16);
    	strokpix.fill(QColor(0,0,0));
    	fillpix.resize(16,16);
    	fillpix.fill(QColor(0,0,0));
    	_StrokeColor->setIconSet(QIconSet( strokpix));
    	_StrokeColor->setTextLabel( trUtf8( "Stroke Color" ) );
        QToolButton *_FillColor= new CToolButton( ToolsOptionsGroup);
        _FillColor->setIconSet(QIconSet( fillpix));
    	_FillColor->setTextLabel( trUtf8( "Fill Color" ) );
    	_StrokeColor->setToggleButton(true);
    	_FillColor->setToggleButton(true);
        	//QHBoxLayout *layer13=new QHBoxLayout(layer12);
    	layer13->addWidget(_StrokeColor);
    	layer13->addWidget(_FillColor); */

    topLayout->addWidget (ToolsButtonGroup);
    topLayout->addWidget (ToolsColorsGroup);
    topLayout->addWidget (ToolsOptionsGroup);

    connect (ArrowTool, SIGNAL (clicked ()), this, SLOT (slotArrowTool ()));
    connect (SubSelectionTool, SIGNAL (clicked ()), this,SLOT (slotSubSelectionTool ()));
    connect (LineTool, SIGNAL (clicked ()), this, SLOT (slotLineTool ()));
    connect (LassoTool, SIGNAL (clicked ()), this, SLOT (slotLassoTool ()));
    connect (PenTool, SIGNAL (clicked ()), this, SLOT (slotPenTool ()));
    connect (TextTool, SIGNAL (clicked ()), this, SLOT (slotTextTool ()));
    connect (OvalTool, SIGNAL (clicked ()), this, SLOT (slotOvalTool ()));
    connect (RectangleTool, SIGNAL (clicked ()), this,SLOT (slotRectangleTool ()));
    connect (PencilTool, SIGNAL (clicked ()), this, SLOT (slotPencilTool ()));
    connect (BrushTool, SIGNAL (clicked ()), this, SLOT (slotBrushTool ()));
    connect (FreeTransformTool, SIGNAL (clicked ()), this,SLOT (slotFreeTransformTool ()));
    connect (FillTransformTool, SIGNAL (clicked ()), this,SLOT (slotFillTransformTool ()));
    connect (InkBottleTool, SIGNAL (clicked ()), this,SLOT (slotInkBottleTool ()));
    connect (PaintBucketTool, SIGNAL (clicked ()), this,SLOT (slotPaintBucketTool ()));
    connect (EyedropperTool, SIGNAL (clicked ()), this,SLOT (slotEyedropperTool ()));
    connect (EraserTool, SIGNAL (clicked ()), this, SLOT (slotEraserTool ()));
    connect (HandTool, SIGNAL (clicked ()), this, SLOT (slotHandTool ()));
    connect (ZoomTool, SIGNAL (clicked ()), this, SLOT (slotZoomTool ()));
    connect (StrokeColor, SIGNAL (clicked ()), this,SLOT (slotStrokeColor ()));
    connect (FillColor, SIGNAL (clicked ()), this, SLOT (slotFillColor ()));

        ////down part for buttons can toggle
    ArrowTool->setCheckable (true);
    SubSelectionTool->setCheckable (true);
    LineTool->setCheckable (true);
    LassoTool->setCheckable (true);
    PenTool->setCheckable (true);
    TextTool->setCheckable (true);
    OvalTool->setCheckable (true);
    RectangleTool->setCheckable (true);
    PencilTool->setCheckable (true);
    BrushTool->setCheckable (true);
    FreeTransformTool->setCheckable (true);
    FillTransformTool->setCheckable (true);
    InkBottleTool->setCheckable (true);
    PaintBucketTool->setCheckable (true);
    EyedropperTool->setCheckable (true);
    EraserTool->setCheckable (true);
    HandTool->setCheckable (true);
    ZoomTool->setCheckable (true);
    StrokeColor->setCheckable (true);
    FillColor->setCheckable (true);

    swatchesCarrier =new CColorPopup (this, "Color swatches Carrier",Qt::Popup | Qt::Dialog);
    swatchesCarrier->hide ();
    swatchesCarrier->setFocus ();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CTools::~CTools (){
        // no need to delete child widgets, Qt does it all for us
}

void CTools::slotArrowTool ()
{
    dad->setDefObjID (1);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotSubSelectionTool ()
{
    dad->setDefObjID (2);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotLineTool ()
{
    dad->setDefObjID (3);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotLassoTool ()
{
    dad->setDefObjID (4);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotPenTool ()
{
    dad->setDefObjID (5);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotTextTool ()
{
    dad->setDefObjID (6);
    dad->properties->hideBrushProperties ();
    dad->properties->showFontProperties ();
}

void CTools::slotOvalTool ()
{
    dad->setDefObjID (7);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotRectangleTool ()
{
    dad->setDefObjID (8);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotPencilTool ()
{
    dad->setDefObjID (9);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotBrushTool ()
{
    dad->setDefObjID (10);
    dad->properties->hideFontProperties ();
    dad->properties->showBrushProperties ();
}

void CTools::slotFreeTransformTool ()
{
    dad->setDefObjID (11);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotFillTransformTool ()
{
    dad->setDefObjID (12);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotInkBottleTool ()
{
    dad->setDefObjID (13);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotPaintBucketTool ()
{
    dad->setDefObjID (14);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotEyedropperTool ()
{
    dad->setDefObjID (15);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotEraserTool ()
{
    dad->setDefObjID (16);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotHandTool ()
{
    dad->setDefObjID (17);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotZoomTool ()
{
    dad->setDefObjID (18);
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotStrokeColor ()
{
    /*QPopupMenu* renkicin = new QPopupMenu (this);
       renkicin->insertItem ("Create Motion Tween");
       renkicin->insertItem(swatchesCarrier);
       renkicin->insertSeparator(); */
    swatchesCarrier->move (mapToGlobal (StrokeColor->pos () * 5));
    swatchesCarrier->resize (230, 170);
    swatchesCarrier->show ();
        //grabMouse();
        //releaseMouse();
        //      grabKeyboard();
        //renkicin->exec();
        //qDebug("stroke button y: %d",StrokeColor->y());
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

void CTools::slotFillColor ()
{
    swatchesCarrier->move (mapToGlobal (FillColor->pos () * 3));
    swatchesCarrier->resize (230, 170);
    swatchesCarrier->show ();
    dad->properties->hideFontProperties ();
    dad->properties->hideBrushProperties ();
}

/////////////////////////////////////////////////////////////
CToolButton::CToolButton (QWidget * parent, const QString &name):
        QToolButton (parent){
    setObjectName (name);
    dad = (CTools *) (parent ? parent->parent () : 0);
}

void CToolButton::enterEvent (QEvent *){
    dad->dad->statusBar ()->showMessage (objectName ());
}

void CToolButton::leaveEvent (QEvent *){
    dad->dad->statusBar ()->showMessage ("Ready.");
}






