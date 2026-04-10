import re

with open('clientes.ui', 'r', encoding='utf-8') as f:
    content = f.read()

# 1. Remove filters from Header
header_pattern = re.compile(r'<widget class="QGroupBox" name="groupBoxFiltros">.*?</widget>\s*</item>', re.DOTALL)
content = header_pattern.sub('', content)

# 2. Add filters back to Ventas tab (tab_4)
# We need to find the layout in tab_4. 
# Looking at previous state, it was in verticalLayout_Ventas -> groupBoxKPIs -> horizontalLayoutKPI
# Let's find horizontalLayoutKPI

ventas_filters = '''          <item>
           <widget class="QLabel" name="labelDesde">
            <property name="text"><string>Desde:</string></property>
           </widget>
          </item>
          <item>
           <widget class="QDateEdit" name="dateEditDesde">
            <property name="minimumSize"><size><width>110</width><height>0</height></size></property>
            <property name="displayFormat"><string>yyyy-MM-dd</string></property>
            <property name="calendarPopup"><bool>true</bool></property>
           </widget>
          </item>
          <item>
           <widget class="QLabel" name="labelHasta">
            <property name="text"><string>Hasta:</string></property>
           </widget>
          </item>
          <item>
           <widget class="QDateEdit" name="dateEditHasta">
            <property name="minimumSize"><size><width>110</width><height>0</height></size></property>
            <property name="displayFormat"><string>yyyy-MM-dd</string></property>
            <property name="calendarPopup"><bool>true</bool></property>
           </widget>
          </item>'''

# Inject before labelTipoGrafico in horizontalLayoutKPI
content = content.replace('<widget class="QLabel" name="labelTipoGrafico">', ventas_filters + '\n          <item>\n           <widget class="QLabel" name="labelTipoGrafico">')

# 3. Move pushButtonRefrescar from Header (it was already removed by step 1) 
# to the bottom button bar after pushModificar

refrescar_btn = '''     <item>
      <widget class="QPushButton" name="pushButtonRefrescar">
       <property name="text"><string>Refrescar</string></property>
       <property name="autoDefault"><bool>false</bool></property>
      </widget>
     </item>'''

content = content.replace('</widget>\n     </item>\n     <item>\n      <spacer name="horizontalSpacerButtons">', 
                          '</widget>\n     </item>\n' + refrescar_btn + '\n     <item>\n      <spacer name="horizontalSpacerButtons">')

with open('clientes.ui', 'w', encoding='utf-8') as f:
    f.write(content)
