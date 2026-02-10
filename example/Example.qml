import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

Window {
    visible: true
    width: 900
    height: 600
    title: qsTr("Tianditu Map Example")
    
    Plugin {
        id: tiandituPlugin
        name: "Tianditu"
        PluginParameter {
            name: "Tianditu.token"
            value: "d4359a63f985f33102eb7b257170dbc0" //! 请替换为您申请的天地图密钥，注册地址：http://lbs.tianditu.gov.cn
        }
    }

    //! 逆地理编码模型
    GeocodeModel {
        id: reverseGeocodeModel
        plugin: tiandituPlugin
        autoUpdate: false
        onLocationsChanged: {
            if (count > 0) {
                var location = get(0)
                reverseResultText.text = qsTr("地址: ") + location.address.text
            }
        }
        onStatusChanged: {
            if (status === GeocodeModel.Error) {
                reverseResultText.text = qsTr("查询出错: ") + errorString
            } else if (status === GeocodeModel.Loading) {
                reverseResultText.text = qsTr("正在查询...")
            }
        }
    }

    //! 地理编码模型（地名搜索）
    GeocodeModel {
        id: geocodeModel
        plugin: tiandituPlugin
        autoUpdate: false
        limit: 5
        onLocationsChanged: {
            searchResultList.model.clear()
            for (var i = 0; i < count; i++) {
                var location = get(i)
                searchResultList.model.append({
                    "name": location.address.text,
                    "lat": location.coordinate.latitude,
                    "lon": location.coordinate.longitude
                })
            }
            if (count === 0) {
                searchStatusText.text = qsTr("未找到结果")
            } else {
                searchStatusText.text = qsTr("找到 ") + count + qsTr(" 个结果")
            }
        }
        onStatusChanged: {
            if (status === GeocodeModel.Error) {
                searchStatusText.text = qsTr("搜索出错: ") + errorString
            } else if (status === GeocodeModel.Loading) {
                searchStatusText.text = qsTr("正在搜索...")
            }
        }
    }

    //! 逆地理编码测试函数
    function reverseGeocode(coordinate) {
        reverseGeocodeModel.query = coordinate
        reverseGeocodeModel.update()
    }

    //! 地名搜索函数
    function searchPlace(placeName) {
        if (placeName.trim() === "") {
            searchStatusText.text = qsTr("请输入地名")
            return
        }
        geocodeModel.query = placeName
        geocodeModel.update()
    }

    //! 跳转到指定坐标
    function gotoLocation(lat, lon) {
        map.center = QtPositioning.coordinate(lat, lon)
        map.zoomLevel = 15
    }

    Map {
        id:map
        plugin: tiandituPlugin
        anchors.fill: parent
        center: QtPositioning.coordinate(39.908828,116.397501) //! 北京天安门
        zoomLevel: 12
        maximumZoomLevel:18
        layer.enabled: true
        layer.samples: 8

        PinchHandler {
            id: pinch
            target: null
            onScaleChanged: (delta) => {
                map.zoomLevel += Math.log2(delta)
            }
            onRotationChanged: (delta) => {
                map.bearing -= delta
            }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel

            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                             ? PointerDevice.Mouse | PointerDevice.TouchPad
                             : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }

        //! 点击地图进行逆地理编码测试
        TapHandler {
            id: tapHandler
            onTapped: (eventPoint, button) => {
                var coord = map.toCoordinate(eventPoint.position)
                reverseGeocode(coord)
            }
        }

        Component.onCompleted: {
           setActiveMap("Tianditu", MapType.SatelliteMapDay); //! 卫星影像图
           //setActiveMap("Tianditu", MapType.TerrainMap);      //! 地形晕渲
           //setActiveMap("Tianditu", MapType.StreetMap);       //! 矢量底图
        }

        function setActiveMap(name,style) {
            for (var i = 0; i < map.supportedMapTypes.length; i++) {
                if (name === map.supportedMapTypes[i].name && style === map.supportedMapTypes[i].style) {
                    map.activeMapType = map.supportedMapTypes[i];
                    return;
                }
            }
        }
    }

    //! 显示逆地理编码结果的面板
    Rectangle {
        id: reverseGeocodePanel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        width: 350
        height: reverseColumn.height + 20
        color: "#E0FFFFFF"
        radius: 8
        border.color: "#CCCCCC"
        border.width: 1
        Column {
            id: reverseColumn
            anchors.centerIn: parent
            spacing: 10    
            Text {
                text: qsTr("点击地图任意位置进行逆地理编码测试")
                font.pixelSize: 14
                color: "#333333"
            }
            
            Text {
                id: reverseResultText
                text: qsTr("等待点击...")
                font.pixelSize: 12
                color: "#666666"
                wrapMode: Text.Wrap
                width: 330
            }
        }
    }

    //! 地名搜索面板
    Rectangle {
        id: searchPanel
        anchors.top: reverseGeocodePanel.bottom
        anchors.left: parent.left
        anchors.margins: 10
        width: 350
        height: searchColumn.height + 20
        color: "#E0FFFFFF"
        radius: 8
        border.color: "#CCCCCC"
        border.width: 1

        Column {
            id: searchColumn
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 10
            spacing: 10

            Text {
                text: qsTr("地名搜索测试")
                font.pixelSize: 14
                font.bold: true
                color: "#333333"
            }

            Row {
                spacing: 8
                width: parent.width

                TextField {
                    id: searchInput
                    width: parent.width - searchButton.width - 8
                    height: 36
                    placeholderText: qsTr("输入地名")
                    font.pixelSize: 14
                    inputMethodHints: Qt.ImhNoAutoUppercase
                    activeFocusOnPress: true
                    selectByMouse: true
                    background: Rectangle {
                        color: "#FFFFFF"
                        border.color: searchInput.activeFocus ? "#1890FF" : "#CCCCCC"
                        border.width: 1
                        radius: 4
                    }
                    onAccepted: searchPlace(searchInput.text)
                    onPressed: {
                        forceActiveFocus()
                        Qt.inputMethod.show()
                    }
                }

                Button {
                    id: searchButton
                    width: 60
                    height: 36
                    text: qsTr("搜索") 
                    onClicked: searchPlace(searchInput.text)
                    background: Rectangle {
                        color: searchButton.pressed ? "#0050B3" : (searchButton.hovered ? "#40A9FF" : "#1890FF")
                        radius: 4
                    }
                    contentItem: Text {
                        text: searchButton.text
                        font.pixelSize: 14
                        color: "#FFFFFF"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Text {
                id: searchStatusText
                text: qsTr("请输入地名进行搜索")
                font.pixelSize: 12
                color: "#666666"
            }

            //! 搜索结果列表
            ListView {
                id: searchResultList
                width: parent.width
                height: Math.min(contentHeight, 200)
                clip: true
                spacing: 4
                model: ListModel {}

                delegate: Rectangle {
                    width: searchResultList.width
                    height: 50
                    color: mouseArea.containsMouse ? "#F0F5FF" : "#FFFFFF"
                    radius: 4
                    border.color: "#E8E8E8"
                    border.width: 1

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            gotoLocation(model.lat, model.lon)
                        }
                    }

                    Column {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 8
                        spacing: 2

                        Text {
                            width: parent.width
                            text: model.name
                            font.pixelSize: 12
                            color: "#333333"
                            elide: Text.ElideRight
                        }

                        Text {
                            text: qsTr("经度: ") + model.lon.toFixed(6) + qsTr("  纬度: ") + model.lat.toFixed(6)
                            font.pixelSize: 10
                            color: "#999999"
                        }
                    }
                }
            }
        }
    }
}