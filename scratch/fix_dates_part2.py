import re

with open('clientes.ui', 'r', encoding='utf-8') as f:
    content = f.read()

# Fix dateEditDesde_2
content = re.sub(
    r'<widget class="QDateEdit" name="dateEditDesde_2">(\s*)<property name="calendarPopup">',
    r'<widget class="QDateEdit" name="dateEditDesde_2">\1 <property name="minimumSize">\1  <size>\1   <width>110</width>\1   <height>0</height>\1  </size>\1 </property>\1 <property name="displayFormat">\1  <string>yyyy-MM-dd</string>\1 </property>\1 <property name="calendarPopup">',
    content
)

# Fix dateEditHasta_2
content = re.sub(
    r'<widget class="QDateEdit" name="dateEditHasta_2">(\s*)<property name="calendarPopup">',
    r'<widget class="QDateEdit" name="dateEditHasta_2">\1 <property name="minimumSize">\1  <size>\1   <width>110</width>\1   <height>0</height>\1  </size>\1 </property>\1 <property name="displayFormat">\1  <string>yyyy-MM-dd</string>\1 </property>\1 <property name="calendarPopup">',
    content
)

with open('clientes.ui', 'w', encoding='utf-8') as f:
    f.write(content)
