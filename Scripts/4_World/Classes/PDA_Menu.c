class PDA_Menu extends UIScriptedMenu
{
    protected string m_LastOpenedApp = "Home";
    protected bool m_IsLoggedIn = false;
    protected bool m_IsRegistered = false;
    protected bool m_StayLoggedIn = true;           // Default = stay logged in

    static bool s_IsLoggedInThisSession = false;

    protected TextWidget m_txtTime;
    protected TextWidget m_txtBattery;
    protected TextWidget m_txtSignal;
    protected TextWidget m_txtDate;
    protected TextWidget m_SystemMessage;

    // Auth screens
    protected Widget m_screenRegister;
    protected Widget m_screenLogin;
    protected Widget m_screenSetPin;
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
            m_screenSetPin   = root.FindAnyWidget("screen_SetPin");
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

            // RPCs
            GetRPCManager().AddRPC("PDA", "RPC_RegisterResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_LoginResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_SetPinResponse", this, SingleplayerExecutionType.Client);

            // Load registration state
            string registered;
            if (GetGame().GetProfileString("PDA_IsRegistered", registered))
                m_IsRegistered = (registered == "true");

            // Starting screen logic
            if (!m_IsRegistered)
            {
                ShowAuthScreen("Register");
            }
            else if (!s_IsLoggedInThisSession)
            {
                ShowAuthScreen("Login");
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
        if (m_screenSetPin)   m_screenSetPin.Show(false);
        if (m_screenPin)      m_screenPin.Show(false);

        if (screenName == "Register" && m_screenRegister) m_screenRegister.Show(true);
        else if (screenName == "Login" && m_screenLogin)  m_screenLogin.Show(true);
        else if (screenName == "SetPin" && m_screenSetPin) m_screenSetPin.Show(true);
        else if (screenName == "Pin" && m_screenPin)      m_screenPin.Show(true);
    }

    void ShowMainScreen(string screenName)
    {
        LockBottomButtons(false);
        m_IsLoggedIn = true;
        s_IsLoggedInThisSession = true;

        if (m_screenRegister) m_screenRegister.Show(false);
        if (m_screenLogin)    m_screenLogin.Show(false);
        if (m_screenSetPin)   m_screenSetPin.Show(false);
        if (m_screenPin)      m_screenPin.Show(false);

        m_LastOpenedApp = screenName;

        if (m_screenHome)     m_screenHome.Show(false);
        if (m_screenMap)      m_screenMap.Show(false);
        if (m_screenContacts) m_screenContacts.Show(false);
        if (m_screenFiles)    m_screenFiles.Show(false);
        if (m_screenNotes)    m_screenNotes.Show(false);
        if (m_screenProfile)  m_screenProfile.Show(false);

        if (screenName == "Home" && m_screenHome)         m_screenHome.Show(true);
        else if (screenName == "Map" && m_screenMap)      m_screenMap.Show(true);
        else if (screenName == "Contacts" && m_screenContacts) m_screenContacts.Show(true);
        else if (screenName == "Files" && m_screenFiles)  m_screenFiles.Show(true);
        else if (screenName == "Notes" && m_screenNotes)  m_screenNotes.Show(true);
        else if (screenName == "Profile" && m_screenProfile) m_screenProfile.Show(true);
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

    // ==================== BUTTON HANDLING ====================
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (button != MouseState.LEFT) return false;

        string name = w.GetName();

        if (name == "btn_Close") { Close(); return true; }

        // Switch between Register and Login
        if (name == "rgr_Login_btn")   { ShowAuthScreen("Login"); return true; }
        if (name == "lgn_Register_btn") { ShowAuthScreen("Register"); return true; }

        // ==================== REGISTER ====================
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

            GetRPCManager().SendRPC("PDA", "RPC_RegisterAccount", new Param2<string, string>(regUser, regPass1), true);
            return true;
        }

        // ==================== SET PIN ====================
        if (name == "pn_SetPin_btn")
        {
            if (!m_pnSetPinCode) return true;

            string pin = m_pnSetPinCode.GetText();
            if (pin == "") { ShowSystemMessage("Please enter a PIN"); return true; }

            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if (player && player.GetIdentity())
            {
                GetRPCManager().SendRPC("PDA", "RPC_SetPin", new Param2<string, string>(player.GetIdentity().GetName(), pin), true);
            }

            ShowSystemMessage("PIN set successfully!");
            ShowAuthScreen("Login");
            return true;
        }

        // ==================== LOGIN ====================
        if (name == "lgn_Login_btn")
        {
            string loginUser = m_lgnName.GetText();
            string loginPass = m_lgnPassword.GetText();

            GetRPCManager().SendRPC("PDA", "RPC_LoginAccount", new Param2<string, string>(loginUser, loginPass), true);
            return true;
        }

        if (!m_IsLoggedIn) return false;

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
            ShowAuthScreen("SetPin");
        }
        else
        {
            ShowSystemMessage("Registration failed. Username may already exist.");
        }
    }

    void RPC_LoginResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("Login successful!");
            m_IsLoggedIn = true;
            s_IsLoggedInThisSession = true;

            // TODO: Later check if user has PIN enabled on server
            ShowMainScreen(m_LastOpenedApp);
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
            ShowSystemMessage("PIN updated successfully!");
        else
            ShowSystemMessage("Failed to update PIN.");
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
    }
}