class PDA_Menu extends UIScriptedMenu
{
    protected string m_LastOpenedApp = "Home";
    protected bool m_IsLoggedIn = false;
    protected bool m_IsRegistered = false;
    protected bool m_StayLoggedIn = true;
    protected bool m_HasSetPIN = false;
    protected bool m_HasSetDisplayName = false;
    protected string m_LastRegisteredUsername = "";
    protected string m_DisplayName = "";
    protected string m_CurrentUsername = "";
    protected string m_SelectedContact = "";
    static bool s_PinEnabled = false;
    static bool s_IsLoggedInThisSession = false;

    protected TextWidget m_txtTime;
    protected TextWidget m_txtBattery;
    protected TextWidget m_txtSignal;
    protected TextWidget m_txtDate;
    protected TextWidget m_SystemMessage;

    // Auth screens
    protected Widget m_screenRegister;
    protected Widget m_screenLogin;
    protected Widget m_screenSetupAccount;
    protected Widget m_screenPin;

    // Main screens + Profile
    protected Widget m_screenHome;
    protected Widget m_screenMap;
    protected Widget m_screenContacts;
    protected Widget m_screenFiles;
    protected Widget m_screenNotes;
    protected Widget m_screenProfile;
    protected Widget m_screen_prf_AccountSettings;
    protected Widget m_screen_prf_Notifications;
    protected Widget m_FunctionButtons;

    // Register
    protected EditBoxWidget m_rgrName;
    protected PasswordEditBoxWidget m_rgrPassword;
    protected PasswordEditBoxWidget m_rgrPasswordAgain;

    // Login
    protected EditBoxWidget m_lgnName;
    protected PasswordEditBoxWidget m_lgnPassword;

    // Set PIN
    protected PasswordEditBoxWidget m_pnSetPinCode;
    protected ButtonWidget m_pnSetPin_btn;

    // Set Display Name
    protected EditBoxWidget m_saSetName_txt;
    protected ButtonWidget m_saSetName_btn;

    // PIN Login
    protected PasswordEditBoxWidget m_pnPinCode;
    protected ButtonWidget m_pnLogin_btn;

    // Profile widgets
    protected TextWidget m_prf_CharacterName;
    protected TextWidget m_prf_FirstLogin;
    protected TextWidget m_prf_TimeSurvived;
    protected TextWidget m_prf_TotalPlayTime;

    protected ButtonWidget m_prf_AccountSettings_btn;
    protected ButtonWidget m_prf_NotificationsSettings_btn;

    protected CheckBoxWidget m_prf_Pin_cb;
    protected CheckBoxWidget m_prf_StayLogedIn_cb;

    protected PasswordEditBoxWidget m_prf_PinOld;
    protected PasswordEditBoxWidget m_prf_PinNew;
    protected ButtonWidget m_prf_PinSet_btn;

    // Contacts screen widgets
    protected TextListboxWidget m_ctcContactList;
    protected ButtonWidget m_ctcContactAdd_btn;
    protected ButtonWidget m_ctcContactRemove_btn;
    protected EditBoxWidget m_ctcContactAdd_txt;

    protected TextListboxWidget m_ctcChatMessages;
    protected EditBoxWidget m_ctcChatSendMsg_txt;
    protected ButtonWidget m_ctcChatSendMsg_btn;

    override Widget Init()
    {
        Widget root = GetGame().GetWorkspace().CreateWidgets("ButchersHell_PDA/data/gui/layouts/pda_desktop.layout");
        
        if (root)
        {
            root.Show(true);

            // Status bar
            m_txtTime       = TextWidget.Cast(root.FindAnyWidget("txt_Time"));
            m_txtBattery    = TextWidget.Cast(root.FindAnyWidget("txt_Battery"));
            m_txtSignal     = TextWidget.Cast(root.FindAnyWidget("txt_Signal"));
            m_txtDate       = TextWidget.Cast(root.FindAnyWidget("txt_Date"));
            m_SystemMessage = TextWidget.Cast(root.FindAnyWidget("syt_SystemMessage_txt"));

            // Auth screens
            m_screenRegister = root.FindAnyWidget("screen_Register");
            m_screenLogin    = root.FindAnyWidget("screen_Login");
            m_screenSetupAccount = root.FindAnyWidget("screen_SetupAccount");
            m_screenPin      = root.FindAnyWidget("screen_Pin");

            // Main screens
            m_screenHome     = root.FindAnyWidget("screen_Home");
            m_screenMap      = root.FindAnyWidget("screen_Map");
            m_screenContacts = root.FindAnyWidget("screen_Contacts");
            m_screenFiles    = root.FindAnyWidget("screen_Files");
            m_screenNotes    = root.FindAnyWidget("screen_Notes");
            m_screenProfile  = root.FindAnyWidget("screen_Profile");

            m_FunctionButtons = root.FindAnyWidget("FunctionButtons");

            // Input fields
            m_rgrName          = EditBoxWidget.Cast(root.FindAnyWidget("rgr_Name_txt"));
            m_rgrPassword      = PasswordEditBoxWidget.Cast(root.FindAnyWidget("rgr_Password_txt"));
            m_rgrPasswordAgain = PasswordEditBoxWidget.Cast(root.FindAnyWidget("rgr_PasswordAgain_txt"));

            m_lgnName     = EditBoxWidget.Cast(root.FindAnyWidget("lgn_Name_txt"));
            m_lgnPassword = PasswordEditBoxWidget.Cast(root.FindAnyWidget("lgn_Password_txt"));

            m_pnSetPinCode = PasswordEditBoxWidget.Cast(root.FindAnyWidget("pn__SetPinCode_txt"));
            m_pnSetPin_btn = ButtonWidget.Cast(root.FindAnyWidget("pn_SetPin_btn"));

            m_saSetName_txt = EditBoxWidget.Cast(root.FindAnyWidget("sa_SetName_txt"));
            m_saSetName_btn = ButtonWidget.Cast(root.FindAnyWidget("sa_SetName_btn"));

            m_pnPinCode = PasswordEditBoxWidget.Cast(root.FindAnyWidget("pn__PinCode_txt"));
            m_pnLogin_btn = ButtonWidget.Cast(root.FindAnyWidget("pn_Login_btn"));

            // Profile widgets
            m_screen_prf_AccountSettings = root.FindAnyWidget("screen_prf_AccountSettings");
            m_screen_prf_Notifications   = root.FindAnyWidget("screen_prf_Notifications");

            m_prf_CharacterName   = TextWidget.Cast(root.FindAnyWidget("prf_CharacterName_txt"));
            m_prf_FirstLogin      = TextWidget.Cast(root.FindAnyWidget("prf_FirstLogin_txt"));
            m_prf_TimeSurvived    = TextWidget.Cast(root.FindAnyWidget("prf_TimeSurvied_txt"));
            m_prf_TotalPlayTime   = TextWidget.Cast(root.FindAnyWidget("prf_TotalPlayTime_txt"));

            m_prf_AccountSettings_btn      = ButtonWidget.Cast(root.FindAnyWidget("prf_AccountSettings_btn"));
            m_prf_NotificationsSettings_btn = ButtonWidget.Cast(root.FindAnyWidget("prf_NotificationsSettings_btn"));

            m_prf_Pin_cb         = CheckBoxWidget.Cast(root.FindAnyWidget("prf_Pin_cb"));
            m_prf_StayLogedIn_cb = CheckBoxWidget.Cast(root.FindAnyWidget("prf_StayLogedIn_cb"));

            m_prf_PinOld    = PasswordEditBoxWidget.Cast(root.FindAnyWidget("prf_PinOld_etxt"));
            m_prf_PinNew    = PasswordEditBoxWidget.Cast(root.FindAnyWidget("prf_PinNew_etxt"));
            m_prf_PinSet_btn = ButtonWidget.Cast(root.FindAnyWidget("prf_PinSet_btn"));

            // Contacts widgets
            m_ctcContactList     = TextListboxWidget.Cast(root.FindAnyWidget("ctc_ContactList"));
            m_ctcContactAdd_btn  = ButtonWidget.Cast(root.FindAnyWidget("ctc_ContactAdd_btn"));
            m_ctcContactRemove_btn = ButtonWidget.Cast(root.FindAnyWidget("ctc_ContactRemove_btn"));
            m_ctcContactAdd_txt  = EditBoxWidget.Cast(root.FindAnyWidget("ctc_ContactAdd_txt"));

            m_ctcChatMessages    = TextListboxWidget.Cast(root.FindAnyWidget("ctc_ChatMessages"));
            m_ctcChatSendMsg_txt = EditBoxWidget.Cast(root.FindAnyWidget("ctc_ChatSendMsg_txt"));
            m_ctcChatSendMsg_btn = ButtonWidget.Cast(root.FindAnyWidget("ctc_ChatSendMsg_btn"));

            // RPCs
            GetRPCManager().AddRPC("PDA", "RPC_RegisterResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_LoginResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_SetPinResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_ChangePinResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_SetPinEnabledResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_VerifyPinResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_SetDisplayNameResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_AddContactResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_RemoveContactResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_GetContactsResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_SendMessageResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_GetMessagesResponse", this, SingleplayerExecutionType.Client);

            // Load registration state
            string registered;
            if (GetGame().GetProfileString("PDA_IsRegistered", registered))
                m_IsRegistered = (registered == "true");

            // Load login state
            string savedUsername;
            if (GetGame().GetProfileString("PDA_CurrentUsername", savedUsername))
            {
                m_CurrentUsername = savedUsername;
            }

            // Starting screen logic
            if (!m_IsRegistered)
            {
                ShowAuthScreen("Register");
            }
            else if (!s_IsLoggedInThisSession)
            {
                ShowAuthScreen("Login");
            }
            else if (s_PinEnabled)
            {
                ShowAuthScreen("Pin");
            }
            else
            {
                ShowMainScreen(m_LastOpenedApp);
            }
        }
        
        return root;
    }

    // ==================== SCREEN MANAGEMENT ====================
    void ShowAuthScreen(string screenName)
    {
        LockBottomButtons(true);

        if (m_screenRegister) m_screenRegister.Show(false);
        if (m_screenLogin)    m_screenLogin.Show(false);
        if (m_screenSetupAccount) m_screenSetupAccount.Show(false);
        if (m_screenPin)      m_screenPin.Show(false);

        if (screenName == "Register" && m_screenRegister) 
            m_screenRegister.Show(true);
        else if (screenName == "Login" && m_screenLogin)  
            m_screenLogin.Show(true);
        else if (screenName == "SetupAccount" && m_screenSetupAccount)   // NEW
            m_screenSetupAccount.Show(true);
        else if (screenName == "Pin" && m_screenPin)      
            m_screenPin.Show(true);
    }

    void ShowMainScreen(string screenName)
    {
        LockBottomButtons(false);
        m_IsLoggedIn = true;
        s_IsLoggedInThisSession = true;

        // Hide Profile sub-screens when switching away from Profile
        if (m_screen_prf_AccountSettings) m_screen_prf_AccountSettings.Show(false);
        if (m_screen_prf_Notifications)   m_screen_prf_Notifications.Show(false);

        // Hide all auth screens
        if (m_screenRegister) m_screenRegister.Show(false);
        if (m_screenLogin)    m_screenLogin.Show(false);
        if (m_screenSetupAccount)   m_screenSetupAccount.Show(false);
        if (m_screenPin)      m_screenPin.Show(false);

        m_LastOpenedApp = screenName;

        // Hide all main screens first
        if (m_screenHome)     m_screenHome.Show(false);
        if (m_screenMap)      m_screenMap.Show(false);
        if (m_screenContacts) m_screenContacts.Show(false);
        if (m_screenFiles)    m_screenFiles.Show(false);
        if (m_screenNotes)    m_screenNotes.Show(false);
        if (m_screenProfile)  m_screenProfile.Show(false);

        // Show the correct screen
        if (screenName == "Home" && m_screenHome)         m_screenHome.Show(true);
        else if (screenName == "Map" && m_screenMap)      m_screenMap.Show(true);
        else if (screenName == "Contacts" && m_screenContacts)
        {
            m_screenContacts.Show(true);
            RequestContacts();
        }
        else if (screenName == "Files" && m_screenFiles)  m_screenFiles.Show(true);
        else if (screenName == "Notes" && m_screenNotes)  m_screenNotes.Show(true);
        else if (screenName == "Profile" && m_screenProfile)
        {
            m_screenProfile.Show(true);
            LoadProfileInfo();
            ShowProfileSubScreen("Account");
        }
        else
        {
            if (m_screenHome) m_screenHome.Show(true);
            m_LastOpenedApp = "Home";
        }
    }

    void ShowProfileSubScreen(string subScreen)
    {
        if (m_screen_prf_AccountSettings) m_screen_prf_AccountSettings.Show(false);
        if (m_screen_prf_Notifications)   m_screen_prf_Notifications.Show(false);

        if (subScreen == "Account" && m_screen_prf_AccountSettings)
            m_screen_prf_AccountSettings.Show(true);
        else if (subScreen == "Notifications" && m_screen_prf_Notifications)
            m_screen_prf_Notifications.Show(true);
    }

    void LoadProfileInfo()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.GetIdentity()) return;

        if (m_prf_CharacterName)
            m_prf_CharacterName.SetText(player.GetIdentity().GetName());

        if (m_prf_FirstLogin)    m_prf_FirstLogin.SetText("First Login: 02/04/2026");
        if (m_prf_TimeSurvived)  m_prf_TimeSurvived.SetText("Time Survived: 4d 18h");
        if (m_prf_TotalPlayTime) m_prf_TotalPlayTime.SetText("Total Playtime: 102h 45m");

        LoadStayLoggedInSetting();
        if (m_prf_Pin_cb)
        {
            m_prf_Pin_cb.SetChecked(s_PinEnabled);
        }
    }

    void LoadStayLoggedInSetting()
    {
        string stayLoggedIn;
        if (GetGame().GetProfileString("PDA_StayLoggedIn", stayLoggedIn))
        {
            m_StayLoggedIn = (stayLoggedIn == "true");
        }

        if (m_prf_StayLogedIn_cb)
        {
            m_prf_StayLogedIn_cb.SetChecked(m_StayLoggedIn);
        }
    }

    void LockBottomButtons(bool lock)
    {
        if (m_FunctionButtons)
            m_FunctionButtons.Enable(!lock);
    }

    // ==================== SYSTEM MESSAGE ====================
    void ShowSystemMessage(string message)
    {
        if (!m_SystemMessage) return;
        m_SystemMessage.SetText(message);
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ClearSystemMessage, 5000, false);
    }

    void ClearSystemMessage()
    {
        if (m_SystemMessage)
            m_SystemMessage.SetText("");
    }

    // ==================== Chat System ====================
    void RequestContacts()
    {
        string username = m_CurrentUsername;

        if (username == "")
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if (player && player.GetIdentity())
                username = player.GetIdentity().GetName();
        }

        if (username != "")
            GetRPCManager().SendRPC("PDA", "RPC_GetContacts", new Param1<string>(username), true);
    }

    void SelectContact()
    {
        if (!m_ctcContactList) return;

        int selectedRow = m_ctcContactList.GetSelectedRow();
        if (selectedRow < 0) return;

        string contactName;
        m_ctcContactList.GetItemText(selectedRow, 0, contactName);

        m_SelectedContact = contactName;

        // Request chat history with this contact
        RequestChatWith(m_SelectedContact);
    }

    void RequestChatWith(string contactName)
    {
        if (contactName == "") return;

        string username = m_CurrentUsername;

        if (username == "")
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if (player && player.GetIdentity())
                username = player.GetIdentity().GetName();
        }

        if (username != "")
        {
            GetRPCManager().SendRPC("PDA", "RPC_GetMessages", new Param2<string, string>(username, contactName), true);
        }
    }

    // ==================== BUTTON HANDLING ====================
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (button != MouseState.LEFT) return false;

        string name = w.GetName();
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer()); // Declared ONLY ONCE here

        if (name == "btn_Close") { Close(); return true; }

        // Refres Contact List 
        if (name == "ctc_ContactList")
        {
            SelectContact();
            return true;
        }

        // Switch between Register and Login
        if (name == "rgr_Login_btn")   { ShowAuthScreen("Login"); return true; }
        if (name == "lgn_Register_btn") { ShowAuthScreen("Register"); return true; }

        // Register
        if (name == "rgr_Register_btn")
        {
            string regUser = m_rgrName.GetText();
            string regPass1 = m_rgrPassword.GetText();
            string regPass2 = m_rgrPasswordAgain.GetText();

            if (regUser == "" || regPass1 == "" || regPass2 == "")
            {
                ShowSystemMessage("Please fill all fields");
                return true;
            }

            if (regPass1 != regPass2)
            {
                ShowSystemMessage("Passwords do not match!");
                return true;
            }

            m_LastRegisteredUsername = regUser;

            GetRPCManager().SendRPC("PDA", "RPC_RegisterAccount", new Param2<string, string>(regUser, regPass1), true);
            return true;
        }

        // Switch from Login screen to Register screen
        if (name == "lgn_Register_btn")
        {
            ShowAuthScreen("Register");
            return true;
        }
        
        // ==================== NORMAL LOGIN ====================
        if (name == "lgn_Login_btn")
        {
            if (!m_lgnName || !m_lgnPassword)
            {
                ShowSystemMessage("Login fields not ready");
                return true;
            }

            string username = m_lgnName.GetText();
            string password = m_lgnPassword.GetText();

            if (username == "" || password == "")
            {
                ShowSystemMessage("Please enter username and password");
                return true;
            }

            m_CurrentUsername = username;
            GetGame().SetProfileString("PDA_CurrentUsername", username);

            GetRPCManager().SendRPC("PDA", "RPC_LoginAccount", new Param2<string, string>(username, password), true);
            return true;
        }

        // Set Account
        if (name == "pn_SetAccount_btn")
        {
            if (!m_HasSetPIN || !m_HasSetDisplayName)
            {
                ShowSystemMessage("Please set both your PIN and Account Name first");
                return true;
            }

            ShowSystemMessage("Setup complete!");
            ShowAuthScreen("Login");
            return true;
        }

        // Set Account Name
        if (name == "sa_SetName_btn")
        {
            if (!m_saSetName_txt) return true;

            string newName = m_saSetName_txt.GetText();
            if (newName == "")
            {
                ShowSystemMessage("Please enter an account name");
                return true;
            }

            string usernameForName = m_LastRegisteredUsername;

            if (usernameForName == "")
            {
                if (player && player.GetIdentity())
                    usernameForName = player.GetIdentity().GetName();
            }

            if (usernameForName != "")
            {
                GetRPCManager().SendRPC("PDA", "RPC_SetDisplayName", 
                    new Param2<string, string>(usernameForName, newName), true);
                
                m_DisplayName = newName;
                m_HasSetDisplayName = true;
                // Removed the local message here → server response will show it
            }
            else
            {
                ShowSystemMessage("Could not determine username");
            }

            return true;
        }

        // Set PIN after registration
        if (name == "pn_SetPin_btn")
        {
            if (!m_pnSetPinCode) return true;

            string pin = m_pnSetPinCode.GetText();
            if (pin == "") { ShowSystemMessage("Please enter a PIN"); return true; }

            string usernameToUse = m_LastRegisteredUsername;

            // Fallback in case the variable is empty
            if (usernameToUse == "")
            {
                if (player && player.GetIdentity())
                    usernameToUse = player.GetIdentity().GetName();
            }

            if (usernameToUse != "")
            {
                GetRPCManager().SendRPC("PDA", "RPC_SetPin", new Param2<string, string>(usernameToUse, pin), true);
            }
            else
            {
                ShowSystemMessage("Could not determine username for PIN");
            }

            m_HasSetPIN = true;
            return true;
        }

        // PIN Login button
        if (name == "pn_Login_btn")
        {
            if (!m_pnPinCode) return true;

            string enteredPin = m_pnPinCode.GetText();
            if (enteredPin == "") 
            { 
                ShowSystemMessage("Please enter your PIN"); 
                return true; 
            }

            string usernameToVerify = m_CurrentUsername;

            // Fallback if we don't have the stored username
            if (usernameToVerify == "")
            {
                if (player && player.GetIdentity())
                    usernameToVerify = player.GetIdentity().GetName();
            }

            if (usernameToVerify != "")
            {
                GetRPCManager().SendRPC("PDA", "RPC_VerifyPin", new Param2<string, string>(usernameToVerify, enteredPin), true);
            }
            else
            {
                ShowSystemMessage("Could not determine account for PIN verification");
            }

            return true;
        }

        // Change PIN
        if (name == "prf_PinSet_btn")
        {
            if (!m_prf_PinOld || !m_prf_PinNew) return true;

            string oldPin = m_prf_PinOld.GetText();
            string updatedPin = m_prf_PinNew.GetText();

            if (oldPin == "" || updatedPin == "")
            {
                ShowSystemMessage("Please fill both PIN fields");
                return true;
            }

            if (player && player.GetIdentity())
            {
                GetRPCManager().SendRPC("PDA", "RPC_ChangePin", 
                    new Param3<string, string, string>(player.GetIdentity().GetName(), oldPin, updatedPin), true);
            }
            return true;
        }

        // PIN Checkbox
        if (name == "prf_Pin_cb")
        {
            if (!m_prf_Pin_cb) return true;

            bool enabled = m_prf_Pin_cb.IsChecked();

            s_PinEnabled = enabled;

            string pinUsername = m_CurrentUsername;
            if (pinUsername == "" && player && player.GetIdentity())
                pinUsername = player.GetIdentity().GetName();

            if (pinUsername != "")
            {
                GetRPCManager().SendRPC("PDA", "RPC_SetPinEnabled", new Param2<string, bool>(pinUsername, enabled), true);
            }

            if (enabled)
            {
                ShowSystemMessage("PIN login enabled");
            }
            else
            {
                ShowSystemMessage("PIN login disabled");
            }

            return true;
        }

        // Stay Logged In Checkbox
        if (name == "prf_StayLogedIn_cb")
        {
            if (m_prf_StayLogedIn_cb)
            {
                m_StayLoggedIn = m_prf_StayLogedIn_cb.IsChecked();

                string stayLoggedInValue = "false";
                if (m_StayLoggedIn) stayLoggedInValue = "true";

                GetGame().SetProfileString("PDA_StayLoggedIn", stayLoggedInValue);

                if (m_StayLoggedIn)
                {
                    ShowSystemMessage("Stay logged in enabled");
                }
                else
                {
                    ShowSystemMessage("Stay logged in disabled");
                }
            }
            return true;
        }

        // ==================== Contact Screen ====================

        // Send Message Button
        if (name == "ctc_ChatSendMsg_btn")
        {
            if (!m_ctcChatSendMsg_txt || m_SelectedContact == "") return true;

            string message = m_ctcChatSendMsg_txt.GetText();
            if (message == "") return true;

            if (!player || !player.GetIdentity()) return true;

            string chatUsername = m_CurrentUsername;
            if (chatUsername == "") chatUsername = player.GetIdentity().GetName();

            GetRPCManager().SendRPC("PDA", "RPC_SendMessage", 
                new Param3<string, string, string>(chatUsername, m_SelectedContact, message), true);

            m_ctcChatSendMsg_txt.SetText(""); // Clear input
            Print("[PDA] Message sent to: " + m_SelectedContact);

            return true;
        }

        // Add Contact
        if (name == "ctc_ContactAdd_btn")
        {
            if (!m_ctcContactAdd_txt) return true;

            string contactName = m_ctcContactAdd_txt.GetText();
            if (contactName == "") return true;

            if (!player || !player.GetIdentity()) return true;

            string addContactUsername = m_CurrentUsername;
            if (addContactUsername == "") addContactUsername = player.GetIdentity().GetName();

            GetRPCManager().SendRPC("PDA", "RPC_AddContact", new Param2<string, string>(addContactUsername, contactName), true);
            m_ctcContactAdd_txt.SetText(""); // Clear input

            return true;
        }

        // Remove Contact
        if (name == "ctc_ContactRemove_btn")
        {
            if (!m_ctcContactList) return true;

            int selectedRow = m_ctcContactList.GetSelectedRow();
            if (selectedRow < 0) return true;

            string contactToRemove;
            m_ctcContactList.GetItemText(selectedRow, 0, contactToRemove);

            if (!player || !player.GetIdentity()) return true;

            string removeContactUsername = m_CurrentUsername;
            if (removeContactUsername == "") removeContactUsername = player.GetIdentity().GetName();

            GetRPCManager().SendRPC("PDA", "RPC_RemoveContact", new Param2<string, string>(removeContactUsername, contactToRemove), true);

            return true;
        }

        if (!m_IsLoggedIn) return false;

        // Profile sub-screens
        if (name == "prf_AccountSettings_btn")       { ShowProfileSubScreen("Account"); return true; }
        if (name == "prf_NotificationsSettings_btn") { ShowProfileSubScreen("Notifications"); return true; }

        // Main screens
        if (name == "btn_Home")     { ShowMainScreen("Home"); return true; }
        if (name == "btn_Map")      { ShowMainScreen("Map"); return true; }
        if (name == "btn_Contacts") { ShowMainScreen("Contacts"); return true; }
        if (name == "btn_Files")    { ShowMainScreen("Files"); return true; }
        if (name == "btn_Notes")    { ShowMainScreen("Notes"); return true; }

        if (name == "btn_Profile")
        {
            ShowMainScreen("Profile");
            LoadProfileInfo();
            ShowProfileSubScreen("Account");
            return true;
        }

        return false;
    }

    // ==================== RPC RESPONSES ====================
    void RPC_RegisterResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("Registration successful! Please set your PIN.");
            ShowAuthScreen("SetupAccount");
        }
        else
        {
            ShowSystemMessage("Registration failed. Username may already exist.");
        }
    }

    void RPC_LoginResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;

        Param3<bool, bool, string> data;     // Changed to Param3
        if (!ctx.Read(data)) return;

        bool success     = data.param1;
        bool requiresPin = data.param2;
        string displayName = data.param3;    // NEW

        if (success)
        {
            ShowSystemMessage("Login successful!");

            m_IsLoggedIn = true;
            s_IsLoggedInThisSession = true;
            m_DisplayName = displayName;     // Save it

            if (requiresPin)
            {
                ShowAuthScreen("Pin");
            }
            else
            {
                ShowMainScreen(m_LastOpenedApp);
            }
        }
        else
        {
            ShowSystemMessage("Login failed. Wrong username or password.");
        }
    }

    void RPC_SetPinResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("PIN set successfully!");
        }
        else
        {
            ShowSystemMessage("Failed to set PIN. Please try again.");
        }
    }

    void RPC_ChangePinResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("PIN changed successfully!");
            m_prf_PinOld.SetText("");
            m_prf_PinNew.SetText("");
        }
        else
        {
            ShowSystemMessage("Failed to change PIN. Wrong old PIN?");
        }
    }

    void RPC_SetPinEnabledResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (!data.param1)
        {
            ShowSystemMessage("Failed to update PIN setting.");
        }
    }

    void RPC_VerifyPinResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("PIN verified!");
            ShowMainScreen(m_LastOpenedApp);
        }
        else
        {
            ShowSystemMessage("Incorrect PIN. Please try again.");
        }
    }

    void RPC_SetDisplayNameResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;

        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("Account name saved successfully!");
        }
        else
        {
            ShowSystemMessage("Failed to save account name.");
        }
    }

    void RPC_GetContactsResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;

        Param1<ref array<string>> data;
        if (!ctx.Read(data)) return;

        if (m_ctcContactList)
        {
            m_ctcContactList.ClearItems();

            array<string> contacts = data.param1;
            foreach (string contact : contacts)
            {
                int row = m_ctcContactList.AddItem(contact, null, 0);
                // You can add logic here later to highlight if unread
            }
        }
    }

    void RPC_AddContactResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        RequestContacts(); // Refresh list
    }

    void RPC_RemoveContactResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        RequestContacts(); // Refresh list
    }

    void RPC_SendMessageResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;

        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("Message sent");

            // Refresh chat if we have a contact selected
            if (m_SelectedContact != "")
            {
                RequestChatWith(m_SelectedContact);
            }
        }
        else
        {
            ShowSystemMessage("Failed to send message");
        }
    }

    void RPC_GetMessagesResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;

        Param1<ref array<ref ChatMessage>> data;
        if (!ctx.Read(data)) return;

        if (!m_ctcChatMessages) return;

        m_ctcChatMessages.ClearItems();

        array<ref ChatMessage> messages = data.param1;

        foreach (ref ChatMessage msg : messages)
        {
            string displayText = msg.from + ": " + msg.message;
            m_ctcChatMessages.AddItem(displayText, null, 0);
        }
    }

    // ==================== OTHER ====================
    override bool OnKeyPress(Widget w, int x, int y, int key)
    {
        if (key == KeyCode.KC_ESCAPE) { Close(); return true; }
        return false;
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);
        UpdateStatusBar();
    }

    void UpdateStatusBar()
    {
        int year, month, day, hour, minute;
        GetGame().GetWorld().GetDate(year, month, day, hour, minute);

        if (m_txtTime)  m_txtTime.SetText(string.Format("%1:%2", hour.ToStringLen(2), minute.ToStringLen(2)));
        if (m_txtDate)  m_txtDate.SetText(string.Format("%1.%2.%3", year, month.ToStringLen(2), day.ToStringLen(2)));

        if (m_txtBattery)
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if (player)
            {
                ItemBase item = player.GetItemInHands();
                if (item)
                {
                    EntityAI battery = item.FindAttachmentBySlotName("BatteryD");
                    if (battery && battery.HasEnergyManager())
                    {
                        int percent = Math.Round(battery.GetCompEM().GetEnergy0To1() * 100);
                        m_txtBattery.SetText("Battery: " + percent + "%");
                    }
                    else m_txtBattery.SetText("Battery: --% (no battery)");
                }
                else m_txtBattery.SetText("Battery: --%");
            }
        }

        if (m_txtSignal) m_txtSignal.SetText("Signal: ●●●○○");
    }

    override void OnShow()
    {
        super.OnShow();
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowCursor(true);

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            player.GetInputController().SetDisabled(true);
            player.GetActionManager().EnableActions(false);
        }
        if (layoutRoot) SetFocus(layoutRoot);
    }

    override void OnHide()
    {
        super.OnHide();
        GetGame().GetInput().ResetGameFocus();

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            player.GetInputController().SetDisabled(false);
            player.GetActionManager().EnableActions(true);
        }

        if (!m_StayLoggedIn)
        {
            s_IsLoggedInThisSession = false;
            m_CurrentUsername = "";
            GetGame().SetProfileString("PDA_CurrentUsername", "");
            Print("[PDA] Stay Logged In OFF → full session reset");
        }
    }
}