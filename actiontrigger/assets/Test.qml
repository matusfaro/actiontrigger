import bb.cascades 1.2
import bb.system 1.2
import QtQuick 1.0

Page {

    property alias prop_listViewOverview: _listViewOverview
    property alias prop_showAll: _listViewOverview.showAll
    property alias prop_viewToggle: viewToggle
    id: _OverviewTab
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    onCreationCompleted: {
        console.log("overviewtab.qml onCreationCompleted")
    }
    function init() {
        dashboardPageTitle.text = "ActionTrigger"
        activity.stop()
    }
        ListView {

    } // end of Container

}
