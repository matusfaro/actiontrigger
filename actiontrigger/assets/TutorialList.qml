import bb.cascades 1.2
import bb.system 1.2

import ActionTrigger 1.0

Page {
    id: pageTutorialList
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    function init() {
        Qt._pageTutorialList = pageTutorialList
        console.log("Populating tutorials")
        /*
         * Available steps (name, constraint):
         * - all
         * 
         * - name, string
         * - group, string
         * - loop, bool
         * - priority, string
         * - favourite, bool
         * - autostart, bool
         * - shortcut
         * 
         * - editTab
         * - settingsTab
         * - runTab
         * 
         * - logTab
         * - variablesTab
         * 
         * - addActionAtEnd, string (regex), string (pretext), string (text)
         * - addActionToBranch, string (regex), string (statementType), string (branchKey)
         * 
         * - openStatement (followed by setRequirement)
         * - setRequirement, string (key), string (value)
         * 
         * - back (From statementPicker or statement page)
         */
        model.insert({
                group: "Basics",
                name: "1. Simple procedure",
                steps: [
                    {
                        action: "name",
                        text: "Welcome to the tutorial. We are going to create a simple procedure to retrieve current time. First off, we need to name our procedure. Let's call it 'Time'",
                        constraint: "Time.*"
                    },
                    {
                        action: "group",
                        text: "For easy navigation, group this procedure under the tutorial group. Type in the word 'Tutorial' for group name.",
                        constraint: "Tutorial.*"
                    },
                    {
                        action: "editTab",
                        text: "Let's get to it and set this up. Click on the 'Edit' tab."
                    },
                    {
                        action: "addActionAtEnd",
                        constraint: "^ACTION_TIME$",
                        pretext: "So far we have no actions or triggers defined. Click the plus button to select one.",
                        text: "Clock->Time(Action)"
                    },
                    {
                        action: "back",
                        text: "You've got it, hit the back button."
                    },
                    {
                        action: "runTab",
                        text: "Now we have a single action that retrieves time. Let's try it out by clicking the 'Run' tab"
                    },
                    {
                        action: "variablesTab",
                        text: "Your procedure has been executed. You can deep dive into exactly what happened by investigating the Run Log. You should watch out for Warnings and Errors.\nClick on the 'Variables' tab"
                    },
                    {
                        action: "settingsTab",
                        text: "Here you can see all the variables that were set as part of the Time action. This is a great way to see what is happening and what variables are available to you. Click on the 'Settings' tab"
                    },
                    {
                        action: "loop",
                        text: "Our last run executed the Time action only once before the procedure finished. To continuously retrieve the time, we need to loop the execution forever. Click this checkbox",
                        constraint: true
                    },
                    {
                        action: "editTab",
                        text: "To save our battery, let's introduce a delay in the loop by adding a short sleep. Click the 'Edit' tab."
                    },
                    {
                        action: "addActionAtEnd",
                        constraint: "^ACTION_SLEEP$",
                        pretext: "Click on the plus sign to add an action at the end.",
                        text: "ExecutionFlow->Sleep(Action)"
                    },
                    {
                        action: "back",
                        text: "Perfect, hit the back button."
                    },
                    {
                        action: "openStatement",
                        text: "Click on the 'Sleep' action to modify the properties.",
                        constraint: "^ACTION_SLEEP$"
                    },
                    {
                        action: "setRequirement",
                        key: "^DURATION$",
                        value: "^2$",
                        text: "Set the Sleep duration to 2 seconds."
                    },
                    {
                        action: "back",
                        text: "That's right, hit the back button."
                    },
                    {
                        action: "runTab",
                        text: "Finally let's try this out. Click the 'Run' tab."
                    },
                    {
                        action: "all",
                        text: "Notice the Variables and Log update every two seconds. You have successfully completed this tutorial."
                    } ]
            })
        model.insert({
                group: "Basics",
                name: "2. Variables",
                steps: [
                    // Initial setup
                    {
                        action: "name",
                        text: "Welcome to the tutorial. Name this procedure 'Variables'",
                        constraint: "Variables.*"
                    },
                    {
                        action: "group",
                        text: "Group this procedure under the 'Tutorial' group name.",
                        constraint: "Tutorial.*"
                    },
                    {
                        action: "editTab",
                        text: "Click on the 'Edit' tab."
                    },
                    // Retrieve all actions/triggers
                    {
                        action: "addActionAtEnd",
                        constraint: "^ACTION_LIGHT$",
                        pretext: "Let's retrieve light information and display it in a message bubble. Click the plus sign.",
                        text: "Sensors->Light(Action)"
                    },
                    {
                        action: "addActionAtEnd",
                        constraint: "^ACTION_ActionDisplayToast$",
                        pretext: "Oops you forgot something. Click the plus sign",
                        text: "Application->Display Message Bubble(Action)"
                    },
                    {
                        action: "back",
                        text: "You've got all of them, hit the back button."
                    },
                    // Setup message bubble
                    {
                        action: "openStatement",
                        text: "The light action sets a variable called ILLUMINANCE. We can use the value of this variable in the Message Bubble. Open 'Display Message Bubble'.",
                        constraint: "^ACTION_ActionDisplayToast$"
                    },
                    {
                        action: "setRequirement",
                        key: "^CONTENT$",
                        value: "^\{ILLUMINANCE\}$",
                        text: "Remove the value for 'Content' and then click the button beside it to select the variable 'ILLUMINANCE'. The content should display '{ILLUMINANCE}'. When the procedure runs, the variable name in curly braces will be replaced by the actual value of the variable."
                    },
                    {
                        action: "back",
                        text: "Hit the back button."
                    },
                    // Try it out
                    {
                        action: "runTab",
                        text: "Finally let's try this out. Click the 'Run' tab."
                    },
                    {
                        action: "variablesTab",
                        text: "The illuminance value retrieved in the Light action is used in the message toast. Take a look at the Variables tab to see the value of all variables."
                    },
                    {
                        action: "all",
                        text: "You have successfully completed this tutorial."
                    } ]
        })
    model.insert({
            group: "Basics",
            name: "3. Branches",
            steps: [
                // Initial setup
                {
                    action: "name",
                    text: "Welcome to the tutorial. Name this procedure 'Branches'",
                    constraint: "Branches.*"
                },
                {
                    action: "group",
                    text: "Group this procedure under the 'Tutorial' group name.",
                    constraint: "Tutorial.*"
                },
                {
                    action: "editTab",
                    text: "Click on the 'Edit' tab."
                },
                // Retrieve IF branch
                {
                    action: "addActionAtEnd",
                    constraint: "^BRANCH_BranchIf$",
                    pretext: "We are going to create an ON-OFF toggle switch for Bluetooth. Click the plus sign.",
                    text: "Execution Flow->If-Then-Else"
                },
                {
                    action: "back",
                    text: "You've got it, hit the back button."
                },
                // Retrieve bluetooth trigger
                { 
                    action: "addActionToBranch",
                    statementType: "^BRANCH_BranchIf$",
                    branchKey: "^IF$",
                    constraint: "^TRIGGER_BLUETOOTH_STATUS$",
                    pretext: "Great, we have a branch in our execution. It consists of one Trigger-only branch 'If'. Depending on the state of the triggers, it will call either the 'Then' action branch or 'Else' branch. Click the plus sign besides 'IF'.",
                    text: "Notice that the selection only includes Triggers. Our trigger will be the Bluetooth status.\nNetwork->Bluetooth Status"
                },
                {
                    action: "back",
                    text: "You've got it, hit the back button."
                },
                // Turn bluetooth off
                {
                    action: "addActionToBranch",
                    statementType: "^BRANCH_BranchIf$",
                    branchKey: "^THEN$",
                    constraint: "^ACTION_BLUETOOTH_POWER$",
                    pretext: "If Bluetooth is on, then turn it off. Click the plus sign besides 'THEN'.",
                    text: "Network->Bluetooth Power"
                },
                {
                    action: "back",
                    text: "You've got it, hit the back button."
                },
                {
                    action: "openStatement",
                    text: "Open 'Bluetooth Power' action.",
                    constraint: "^ACTION_BLUETOOTH_POWER$"
                },
                {
                    action: "setRequirement",
                    key: "^STATE$",
                    value: "^off$",
                    text: "Set the desired state to OFF"
                },
                {
                    action: "back",
                    text: "Hit the back button."
                },
                // Turn bluetooth on
                { 
                    action: "addActionToBranch",
                    statementType: "^BRANCH_BranchIf$",
                    branchKey: "^ELSE$",
                    constraint: "^ACTION_BLUETOOTH_POWER$",
                    pretext: "Otherwise, when Bluetooth is off, then turn it back on. Click the plus sign besides 'ELSE'.",
                    text: "Network->Bluetooth Power"
                },
                {
                    action: "back",
                    text: "You've got it, hit the back button."
                },
                // Try it out
                {
                    action: "runTab",
                    text: "Let's try this out. Click the 'Run' tab."
                },
                {
                    action: "all",
                    text: "Bluetooth should have been toggled. You have successfully completed this tutorial."
                } ]
    })
        model.insert({
                group: "Examples",
                name: "Persistent Variables",
                description: "Keep a counter in persistent storage how many times you have flipped your device upside down",
                steps: [
                    // Initial setup
                    {
                        action: "name",
                        text: "Welcome to the tutorial. Name this procedure 'Persistent'",
                        constraint: "Persistent.*"
                    },
                    {
                        action: "group",
                        text: "Group this procedure under the 'Tutorial' group name.",
                        constraint: "Tutorial.*"
                    },
                    {
                        action: "loop",
                        text: "We want this procedure to continuously run in a loop. Click this checkbox",
                        constraint: true
                    },
                    {
                        action: "editTab",
                        text: "Click on the 'Edit' tab."
                    },
                    // Retrieve all actions/triggers
                    {
                        action: "addActionAtEnd",
                        constraint: "^TRIGGER_COMPASS$",
                        pretext: "Let's create a procedure that will count the amount of times you flip the device upside down and then display the counter in a message bubble. We will store the counter in persistent storage to preserve the value across restarts. Click the plus sign.",
                        text: "Sensors->Compass(Trigger)"
                    },
                    {
                        action: "addActionAtEnd",
                        constraint: "^ACTION_ActionGetPersistentVar$",
                        pretext: "Oops you forgot something. Click the plus sign",
                        text: "Application->Get Persistent Variable(Action)"
                    },
                    {
                        action: "addActionAtEnd",
                        constraint: "^ACTION_ActionMath$",
                        pretext: "Oops you forgot something. Click the plus sign",
                        text: "Application->Math Operations(Action)"
                    },
                    {
                        action: "addActionAtEnd",
                        constraint: "^ACTION_ActionSetPersistentVar$",
                        text: "Application->Set Persistent Variable(Action)"
                    },
                    {
                        action: "addActionAtEnd",
                        constraint: "^ACTION_ActionDisplayToast$",
                        pretext: "Oops you forgot something. Click the plus sign",
                        text: "Application->Display Message Bubble(Action)"
                    },
                    {
                        action: "back",
                        text: "You've got all of them, hit the back button."
                    },
                    // Modify compass
                    {
                        action: "openStatement",
                        text: "First we need to wait until the device is flipped. Open 'Compass' trigger.",
                        constraint: "^TRIGGER_COMPASS$"
                    },
                    {
                        action: "setRequirement",
                        key: "^FACE_DOWN_REQUIREMENT$",
                        value: "^CHANGED$",
                        text: "Set the Face Down requirement to detect change"
                    },
                    {
                        action: "back",
                        text: "Hit the back button."
                    },
                    // Read from persistent storage
                    {
                        action: "openStatement",
                        text: "What we will do is retrieve the device flip count from persistent storage, increment it and then store the value back into storage. Open 'Get Persistent Variable' action.",
                        constraint: "^ACTION_ActionGetPersistentVar$"
                    },
                    {
                        action: "setRequirement",
                        key: "^PERSISTENT_NAME$",
                        value: "^CountGlobal$",
                        text: "Let's call the persistent variable name 'CountGlobal'."
                    },
                    {
                        action: "setRequirement",
                        key: "^LOCAL_NAME$",
                        value: "^CountLocal$",
                        text: "Read the value into the local variable name 'CountLocal'"
                    },
                    {
                        action: "setRequirement",
                        key: "^DEFAULT_VALUE$",
                        value: "^0$",
                        text: "If the global variable happens to be empty, the default value for the flip count should be '0'."
                    },
                    {
                        action: "back",
                        text: "Good, hit the back button."
                    },
                    // Increment
                    {
                        action: "openStatement",
                        text: "Now that we retrieved the count value, we need to increment it by one. Open 'Math operations' action.",
                        constraint: "^ACTION_ActionMath$"
                    },
                    {
                        action: "setRequirement",
                        key: "^X$",
                        value: "^\{CountLocal\}$",
                        text: "We will use the local variable in the last action as the X value. Remove the value for X and click the button beside it to select the local variable 'CountLocal'. The value of X should be '{CountLocal}'. When the procedure runs, the variable name in curly braces will be replaced by the actual value of the variable."
                    },
                    {
                        action: "setRequirement",
                        key: "^Y$",
                        value: "^1$",
                        text: "You got it. Now set the Y value to '1' to increment the counter by one."
                    },
                    {
                        action: "back",
                        text: "Good, hit the back button."
                    },
                    // Store to persistent storage
                    {
                        action: "openStatement",
                        text: "Great! Now that we incremented the value, we need to store it back into persistent storage. Open 'Set Persistent Variable' action.",
                        constraint: "^ACTION_ActionSetPersistentVar$"
                    },
                    {
                        action: "setRequirement",
                        key: "^NAME$",
                        value: "^CountGlobal$",
                        text: "Set the global name to 'CountGlobal', the same name we used earlier."
                    },
                    {
                        action: "setRequirement",
                        key: "^CONTENT$",
                        value: "^\{MATH_OUTPUT\}$",
                        text: "Now we need to store the incremented value. Under 'Content', click the button beside it and select the variable 'MATH_OUTPUT' which contains the result from the previous action. The content should display '{MATH_OUTPUT}'."
                    },
                    {
                        action: "back",
                        text: "Good, hit the back button."
                    },
                    // Store to persistent storage
                    {
                        action: "openStatement",
                        text: "Finally, let's show the counter value in a message bubble for us to see. Open 'Display Message Bubble' action.",
                        constraint: "^ACTION_ActionDisplayToast$"
                    },
                    {
                        action: "setRequirement",
                        key: "^CONTENT$",
                        value: "^\{MATH_OUTPUT\}$",
                        text: "Remove the value for 'Content' and then click the button beside it to select the variable 'MATH_OUTPUT'. The content should display '{MATH_OUTPUT}'."
                    },
                    {
                        action: "back",
                        text: "Good, hit the back button."
                    },
                    // Try it out
                    {
                        action: "runTab",
                        text: "Finally let's try this out. Click the 'Run' tab."
                    },
                    {
                        action: "variablesTab",
                        text: "Now try flipping your device face down and face up. You should see the message bubble pop up. Take a look under Variables to observe the local variables you have used."
                    },
                    {
                        action: "all",
                        text: "You have successfully completed this tutorial."
                    } ]
            })
        model.insert({
                group: "Examples",
                name: "What time is it?",
                description: "Read incoming messages and reply with current time when asked 'What time is it?'",
                steps: [
                    // Initial setup
                    {
                        action: "name",
                        text: "Welcome to the tutorial. Name this procedure 'Clock'",
                        constraint: "Clock.*"
                    },
                    {
                        action: "group",
                        text: "Group this procedure under the 'Tutorial' group name.",
                        constraint: "Tutorial.*"
                    },
                    {
                        action: "loop",
                        text: "We want this procedure to continuously run in a loop. Click this checkbox",
                        constraint: true
                    },
                    {
                        action: "editTab",
                        text: "Click on the 'Edit' tab."
                    },
                    // Retrieve all actions/triggers
                    {
                        action: "addActionAtEnd",
                        constraint: "^TRIGGER_TriggerReceiveOpenMessage$",
                        pretext: "Let's setup the phone to automatically respond whenever it receives a message asking 'What time is it?'. Click the plus sign.",
                        text: "Hub->Receive And Open Message"
                    },
                    {
                        action: "addActionAtEnd",
                        constraint: "^BRANCH_BranchIfThen$",
                        pretext: "Oops you forgot something. Click the plus sign",
                        text: "Execution Flow->If-Then"
                    },
                    {
                        action: "back",
                        text: "You've got all of them, hit the back button."
                    },
                    // Setup regex match
                    {
                        action: "addActionToBranch",
                        statementType: "^BRANCH_BranchIfThen$",
                        branchKey: "^IF$",
                        constraint: "^TRIGGER_TriggerCustom$",
                        pretext: "We are now receiving all incoming messages. Let's read all of the messages and check if it contains the sentence 'What time is it?'. Click the plus sign besides 'IF'.",
                        text: "Application->Variable Trigger"
                    },
                    {
                        action: "back",
                        text: "You've got it, hit the back button."
                    },
                    {
                        action: "openStatement",
                        text: "Open 'Variable Trigger'.",
                        constraint: "^TRIGGER_TriggerCustom$"
                    },
                    {
                        action: "setRequirement",
                        key: "^INPUT$",
                        value: ".*\{MSG_RECV_BODY\}.*\{MSG_RECV_SUBJECT\}.*|.*\{MSG_RECV_SUBJECT\}.*\{MSG_RECV_BODY\}.*",
                        text: "We want to match both the subject and the content of the message. Click the button beside 'Input' to select the variables 'MSG_RECV_BODY' and 'MSG_RECV_SUBJECT'. It should display '{MSG_RECV_BODY}{MSG_RECV_SUBJECT}'."
                    },
                    {
                        action: "setRequirement",
                        key: "^INPUT_REQUIREMENT$",
                        value: "^CONTAINS$",
                        text: "Under 'Requirement' select 'Contains' requirement."
                    },
                    {
                        action: "setRequirement",
                        key: "^INPUT_MATCH",
                        value: "^What time is it\\?$",
                        text: "Type 'What time is it?' under 'Contains' without any extra letters or spaces. Whenever this sentence is part of the subject and/or content of the message, it will trigger."
                    },
                    {
                        action: "back",
                        text: "Hit the back button."
                    },
                    // Setup finder
                    {
                        action: "addActionToBranch",
                        statementType: "^BRANCH_BranchIfThen$",
                        branchKey: "^THEN$",
                        constraint: "^ACTION_TIME$",
                        pretext: "Now that we have our trigger, we need to retrieve the time and reply. Click the plus sign besides 'THEN'.",
                        text: "Clock->Time"
                    },
                    {
                        action: "addActionToBranch",
                        statementType: "^BRANCH_BranchIfThen$",
                        branchKey: "^THEN$",
                        constraint: "^ACTION_ActionSendMessage$",
                        pretext: "Oops, you forgot something. Click the plus sign besides 'THEN'.",
                        text: "Hub->Send Message"
                    },
                    {
                        action: "back",
                        text: "You've got it, hit the back button."
                    },
                    {
                        action: "openStatement",
                        text: "Let's customize the reply message. Open 'Send Message'.",
                        constraint: "^ACTION_ActionSendMessage$"
                    },
                    {
                        action: "setRequirement",
                        key: "^ACCOUNT_KEY$",
                        value: "^\{MSG_ACCOUNT_ID\}$",
                        text: "We need to reply back with the correct account. Set the Account to the variable MSG_ACCOUNT_ID. It should display '{MSG_ACCOUNT_ID}'."
                    },
                    {
                        action: "setRequirement",
                        key: "^RECIPIENT$",
                        value: "^\{MSG_RECV_RECIPIENT_From_1_ADDRESS\}$",
                        text: "We need to reply back to the correct person. Set the field 'To' to the variable MSG_RECV_RECIPIENT_From_1_ADDRESS that contains the address of the person that sent us the initial message. It should display '{MSG_RECV_RECIPIENT_From_1_ADDRESS}'."
                    },
                    {
                        action: "setRequirement",
                        key: "^CONTENT$",
                        value: ".*\{TIME\}.*",
                        text: "Finally the 'Body' of the message should contain the time. Feel free to write a message in the Body and include the time variable. Example: 'The time is {TIME}'."
                    },
                    {
                        action: "back",
                        text: "Hit the back button."
                    },
                    // Try it out
                    {
                        action: "runTab",
                        text: "Finally let's try this out. Click the 'Run' tab."
                    },
                    {
                        action: "all",
                        text: "You can try this out by sending yourself an email, PIN, or text (SMS) message that contains 'What time is it?'. You have successfully completed this tutorial."
                    } ]
            })
        activity.stop()
    }
    DefaultPageContainer {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        DefaultContainer {
            DefaultActivity {
                id: activity
            }
            Label {
                text: "Tutorials"
                textStyle.base: SystemDefaults.TextStyles.BigText
            }
        }
        // PARAMETER DESCRIPTION
        Label {
            multiline: true
            textStyle.base: SystemDefaults.TextStyles.SubtitleText
            text: "Enjoy these tutorials to learn more"
        }
        ListView {
            id: _listView_tutorials
            scrollRole: ScrollRole.Main
            dataModel: RegexDataModel {
                id: model
                grouping: ItemGrouping.ByFullValue
                sortingKeys: [ 'group', 'name' ]
            }
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    NonStandardListItem {
                        id: root

                        title: ListItemData.name
                        description: ListItemData.description
                        select.onTapped: {
                            if (! Qt._premium.askPurchase()) {
                                return
                            }
                            root.ListItem.view.getActivity().start()
                            var page = Qt._procedurePage.createObject()
                            page.tutorialSteps = ListItemData.steps
                            page.isNew = true
                            Qt.ProcedurePageProcedureView = Qt._actiontrigger.createProcedure()
                            root.ListItem.view.getActivity().stop()
                            Qt._pane.push(page);
                        }
                    }
                }
            ]
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2.5
            }
            function getActivity() {
                return activity
            }
        }
    }

    actions: [
//        ActionItem {
//            ActionBar.placement: ActionBarPlacement.OnBar
//            title: "Help"
//            imageSource: "asset:///images/bb10/ic_help.png"
//            onTriggered: {
//                var page = helpSheet.createObject()
//                page.topic = "selectVariable"
//                page.open()
//            }
//        }
    ]
}
