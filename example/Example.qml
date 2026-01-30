import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

Window {
    visible: true
    width: 800
    height: 600
    title: qsTr("Tianditu Map Example")
    
    Plugin {
        id: taindituPlugin
        name: "Tianditu"
        PluginParameter {
            name: "Tianditu.token"
            value: "d4359a63f985f33102eb7b257170dbc0" //! 请替换为您申请的天地图密钥，注册地址：http://lbs.tianditu.gov.cn
        }
    }

    //! 逆地理编码模型
    GeocodeModel {
        id: geocodeModel
        plugin: taindituPlugin
        autoUpdate: false
        onLocationsChanged: {
            if (count > 0) {
                var location = get(0)
                resultText.text = "地址: " + location.address.text
            }
        }
        onStatusChanged: {
            if (status === GeocodeModel.Error) {
                resultText.text = "查询出错: " + errorString
            } else if (status === GeocodeModel.Loading) {
                resultText.text = "正在查询..."
            }
        }
    }

    //! 逆地理编码测试函数
    function reverseGeocode(coordinate) {
        geocodeModel.query = coordinate
        geocodeModel.update()
    }

    Map {
        id:map
        plugin: taindituPlugin
        anchors.fill: parent
        center: QtPositioning.coordinate(39.908828,116.397501) //! 北京天安门
        zoomLevel: 10
        maximumZoomLevel:18
        layer.enabled: true
        layer.samples: 8

        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
            }
            onScaleChanged: (delta) => {
                map.zoomLevel += Math.log2(delta)
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
            onRotationChanged: (delta) => {
                map.bearing -= delta
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel
            // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
            // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
            // and we don't yet distinguish mice and trackpads on Wayland either
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
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        width: 350
        height: column.height + 20
        color: "#E0FFFFFF"
        radius: 8
        border.color: "#CCCCCC"
        border.width: 1
        Column {
            id: column
            anchors.centerIn: parent
            spacing: 10    
            Text {
                text: "点击地图任意位置进行逆地理编码测试"
                font.pixelSize: 14
                color: "#333333"
            }
            
            Text {
                id: resultText
                text: "等待点击..."
                font.pixelSize: 12
                color: "#666666"
                wrapMode: Text.Wrap
                width: 330
            }
        }
    }
}