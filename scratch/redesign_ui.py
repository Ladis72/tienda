import re
import sys

with open('clientes.ui', 'r', encoding='utf-8') as f:
    content = f.read()

# 1. Update Dialog Style and Layout Start
content = re.sub(
    r'<widget class="QDialog" name="Clientes">\s*<property name="geometry">',
    r'<widget class="QDialog" name="Clientes">\n  <property name="styleSheet">\n   <string notr="true">QDialog { background-color: #f8f9fa; }</string>\n  </property>\n  <property name="geometry">',
    content
)

# Replace the grid layout with a vertical layout and add the header frame
old_layout_pattern = re.compile(r'  <layout class="QGridLayout" name="gridLayout">.*?</layout>', re.DOTALL)
# This is tricky because the layout contains the tab widget.

# Let's do it differently: Match from the start of the layout to the first <item row="1"
content = re.sub(
    r'  <layout class="QGridLayout" name="gridLayout">.*?<item row="1" column="0">',
    r'''  <layout class="QVBoxLayout" name="mainVerticalLayout">
   <property name="spacing">
    <number>15</number>
   </property>
   <property name="leftMargin">
    <number>20</number>
   </property>
   <property name="topMargin">
    <number>20</number>
   </property>
   <property name="rightMargin">
    <number>20</number>
   </property>
   <property name="bottomMargin">
    <number>20</number>
   </property>
   <item>
    <widget class="QFrame" name="headerFrame">
     <property name="minimumSize">
      <size>
       <width>0</width>
       <height>80</height>
      </size>
     </property>
     <property name="styleSheet">
      <string notr="true">QFrame#headerFrame { 
    background-color: #e8f5e9; 
    border-radius: 10px; 
    border: 1px solid #c8e6c9;
}</string>
     </property>
     <layout class="QHBoxLayout" name="horizontalLayoutHeader">
      <item>
       <widget class="QLabel" name="labelIcono">
        <property name="maximumSize">
         <size>
          <width>48</width>
          <height>48</height>
         </size>
        </property>
        <property name="text"><string/></property>
        <property name="pixmap"><pixmap>../iconos/clientes.png</pixmap></property>
        <property name="scaledContents"><bool>true</bool></property>
       </widget>
      </item>
      <item>
       <widget class="QLabel" name="labelNombreCliente">
        <property name="font">
         <font><pointsize>18</pointsize><bold>true</bold></font>
        </property>
        <property name="styleSheet"><string notr="true">color: #2e7d32;</string></property>
        <property name="text"><string>Cliente</string></property>
       </widget>
      </item>
      <item>
       <spacer name="horizontalSpacerHeader">
        <property name="orientation"><enum>Qt::Orientation::Horizontal</enum></property>
        <property name="sizeHint" stdset="0"><size><width>40</width><height>20</height></size></property>
       </spacer>
      </item>
      <item>
       <widget class="QGroupBox" name="groupBoxFiltros">
        <property name="title"><string>Filtro de Fechas Global</string></property>
        <layout class="QHBoxLayout" name="horizontalLayoutFiltros">
         <item>
          <widget class="QLabel" name="label_17"><property name="text"><string>Desde:</string></property></widget>
         </item>
         <item>
          <widget class="QDateEdit" name="dateEditDesde">
           <property name="minimumSize"><size><width>110</width><height>0</height></size></property>
           <property name="displayFormat"><string>yyyy-MM-dd</string></property>
           <property name="calendarPopup"><bool>true</bool></property>
          </widget>
         </item>
         <item>
          <widget class="QLabel" name="label_18"><property name="text"><string>Hasta:</string></property></widget>
         </item>
         <item>
          <widget class="QDateEdit" name="dateEditHasta">
           <property name="minimumSize"><size><width>110</width><height>0</height></size></property>
           <property name="displayFormat"><string>yyyy-MM-dd</string></property>
           <property name="calendarPopup"><bool>true</bool></property>
          </widget>
         </item>
         <item>
          <widget class="QPushButton" name="pushButtonRefrescar"><property name="text"><string>Refrescar</string></property></widget>
         </item>
        </layout>
       </widget>
      </item>
     </layout>
    </widget>
   </item>
   <item>
    <widget class="QTabWidget" name="tabWidgetProductos">''',
    content,
    flags=re.DOTALL
)

# 2. Update Tab Widget Titles with icons
content = content.replace('<string>General</string>', '<string>👤 Datos Generales</string>')
content = content.replace('<string>Ventas</string>', '<string>📊 Historial de Ventas</string>')
content = content.replace('<string>Productos</string>', '<string>📦 Productos Comprados</string>')

# 3. Finalize button bar and close layouts
# Extract the button code to reuse it in a cleaner layout if needed, or just port it.
content = re.sub(
    r'    </widget>\s*</item>\s*<item row="0" column="0">.*?</layout>',
    r'''    </widget>
   </item>
   <item>
    <layout class="QHBoxLayout" name="horizontalLayoutButtons">
     <item>
      <widget class="QPushButton" name="pushButtonNuevo"><property name="text"><string>Nuevo</string></property><property name="autoDefault"><bool>false</bool></property></widget>
     </item>
     <item>
      <widget class="QPushButton" name="pushButtonBorrar"><property name="enabled"><bool>false</bool></property><property name="text"><string>Borrar</string></property><property name="autoDefault"><bool>false</bool></property></widget>
     </item>
     <item>
      <widget class="QPushButton" name="pushButtonModificar"><property name="text"><string>Modificar</string></property><property name="autoDefault"><bool>false</bool></property></widget>
     </item>
     <item>
      <spacer name="horizontalSpacerButtons">
       <property name="orientation"><enum>Qt::Orientation::Horizontal</enum></property>
       <property name="sizeHint" stdset="0"><size><width>40</width><height>20</height></size></property>
      </spacer>
     </item>
     <item>
      <widget class="QPushButton" name="pushButtonAnterior"><property name="text"><string>Anterior</string></property><property name="autoDefault"><bool>false</bool></property></widget>
     </item>
     <item>
      <widget class="QPushButton" name="pushButtonSiguiente"><property name="text"><string>Siguiente</string></property><property name="autoDefault"><bool>false</bool></property></widget>
     </item>
     <item>
      <widget class="QPushButton" name="btn_encargos_cliente"><property name="text"><string>Encargos</string></property><property name="autoDefault"><bool>false</bool></property></widget>
     </item>
     <item>
      <widget class="QPushButton" name="pushButtonCerrar"><property name="text"><string>Cerrar</string></property><property name="autoDefault"><bool>false</bool></property></widget>
     </item>
    </layout>
   </item>
  </layout>''',
    content,
    flags=re.DOTALL
)

with open('clientes.ui', 'w', encoding='utf-8') as f:
    f.write(content)
