import xml.etree.ElementTree as ET

tree = ET.parse('tiendas.ui')
root = tree.getroot()

grid_layout = root.find(".//layout[@name='gridLayout']")

# 1. Modify general_2 item
for item in grid_layout.findall("item"):
    widget = item.find("widget")
    if widget is not None and widget.get("name") == "general_2":
        item.set("column", "1")
        item.set("colspan", "7")

# 2. Add tableViewTiendas
new_item = ET.Element("item", {"row": "0", "column": "0", "rowspan": "3", "colspan": "1"})
table_widget = ET.SubElement(new_item, "widget", {"class": "QTableView", "name": "tableViewTiendas"})
ET.SubElement(table_widget, "property", {"name": "selectionBehavior"}).append(ET.Element("enum", text="QAbstractItemView::SelectRows"))
grid_layout.append(new_item)

# 3. Modify buttons layout
for item in grid_layout.findall("item"):
    layout = item.find("layout")
    if layout is not None and layout.get("name") == "horizontalLayout":
        # Remove old buttons
        items_to_remove = []
        for hw_item in layout.findall("item"):
            btn = hw_item.find("widget")
            if btn is not None and btn.get("name") in ["pushButtonModificar", "pushButtonAnterior", "pushButtonSiguiente"]:
                items_to_remove.append(hw_item)
        for hw_item in items_to_remove:
            layout.remove(hw_item)
        
        # Add Guardar and Cancelar
        for btn_name, btn_text in [("pushButtonGuardar", "Guardar"), ("pushButtonCancelar", "Cancelar")]:
            btn_item = ET.Element("item")
            btn_widget = ET.SubElement(btn_item, "widget", {"class": "QPushButton", "name": btn_name})
            ET.SubElement(btn_widget, "property", {"name": "text"}).append(ET.Element("string", text=btn_text))
            
            # Hide them by default setup in ui compiler? No, we will hide them via code.
            # Insert them before 'Borrar' or 'Refrescar'
            # Let's just append them at the beginning (after Nuevo)
            layout.insert(1, btn_item)

tree.write('tiendas.ui', xml_declaration=True, encoding='UTF-8')
print("UI rewritten successfully.")
