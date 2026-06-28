class PDA_Menu extends UIScriptedMenu
{
    protected string m_LastOpenedApp = "Home";
    protected bool m_IsLoggedIn = false;
    protected bool m_IsRegistered = false;

    static bool s_IsLoggedInThisSession = false;

    protected TextWidget m_txtTime;
    protected TextWidget m_txtBattery;
    protected TextWidget m_txtSignal;
    protected TextWidget m_txtDate;
    protected TextWidget m_SystemMessage;

    protected Widget m_screenRegister;
    protected Widget m_screenLogin;
    protected Widget m_screenPin;
    protected Widget m_screenHome;
    protected Widget m_screenMap;
    protected Widget m_screenContacts;
    protected Widget m_screenFiles;
    protected Widget m_screenNotes;
    protected Widget m_screenProfile;

    protected Widget m_FunctionButtons;

    protected EditBoxWidget m_rgrName;
    protected EditBoxWidget m_rgrPassword;
    protected EditBoxWidget m_rgrPasswordAgain;
    protected EditBoxWidget m_lgnName;
    protected EditBoxWidget m_lgnPassword;

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
            m_rgrPassword      = EditBoxWidget.Cast(root.FindAnyWidget("rgr_Password_txt"));
            m_rgrPasswordAgain = EditBoxWidget.Cast(root.FindAnyWidget("rgr_PasswordAgain_txt"));
            m_lgnName          = EditBoxWidget.Cast(root.FindAnyWidget("lgn_Name_txt"));
            m_lgnPassword      = EditBoxWidget.Cast(root.FindAnyWidget("lgn_Password_txt"));

            // Register RPC responses
            GetRPCManager().AddRPC("PDA", "RPC_RegisterResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_LoginResponse", this, SingleplayerExecutionType.Client);

            // Load registration state
            string registered;
            if (GetGame().GetProfileString("PDA_IsRegistered", registered))
                m_IsRegistered = (registered == "true");

            // Decide starting screen
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

    // ==================== SCREEN CONTROL ====================
    void ShowAuthScreen(string screenName)
    {
        LockBottomButtons(true);

        if (m_screenRegister) m_screenRegister.Show(false);
        if (m_screenLogin)    m_screenLogin.Show(false);
        if (m_screenPin)      m_screenPin.Show(false);

        if (screenName == "Register" && m_screenRegister) m_screenRegister.Show(true);
        else if (screenName == "Login" && m_screenLogin)  m_screenLogin.Show(true);
        else if (screenName == "Pin" && m_screenPin)      m_screenPin.Show(true);
    }

    void ShowMainScreen(string screenName)
    {
        LockBottomButtons(false);
        m_IsLoggedIn = true;
        s_IsLoggedInThisSession = true;

        if (m_screenRegister) m_screenRegister.Show(false);
        if (m_screenLogin)    m_screenLogin.Show(false);
        if (m_screenPin)      m_screenPin.Show(false);

        m_LastOpenedApp = screenName;

        // Save last screen to server
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player && player.GetIdentity())
        {
            GetRPCManager().SendRPC("PDA", "RPC_SaveLastScreen", 
                new Param2<string, string>(player.GetIdentity().GetName(), screenName), true);
        }

        // Hide all main screens
        if (m_screenHome)     m_screenHome.Show(false);
        if (m_screenMap)      m_screenMap.Show(false);
        if (m_screenContacts) m_screenContacts.Show(false);
        if (m_screenFiles)    m_screenFiles.Show(false);
        if (m_screenNotes)    m_screenNotes.Show(false);
        if (m_screenProfile)  m_screenProfile.Show(false);

        // Show correct screen
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

        // Register
        if (name == "rgr_Register_btn")
        {
            string u = m_rgrName.GetText();
            string p1 = m_rgrPassword.GetText();
            string p2 = m_rgrPasswordAgain.GetText();

            if (u == "" || p1 == "" || p2 == "") { ShowSystemMessage("Please fill all fields"); return true; }
            if (p1 != p2) { ShowSystemMessage("Passwords do not match!"); return true; }

            GetRPCManager().SendRPC("PDA", "RPC_RegisterAccount", new Param2<string, string>(u, p1), true);
            return true;
        }

        // Login
        if (name == "lgn_Login_btn")
        {
            string u = m_lgnName.GetText();
            string p = m_lgnPassword.GetText();
            GetRPCManager().SendRPC("PDA", "RPC_LoginAccount", new Param2<string, string>(u, p), true);
            return true;
        }

        if (!m_IsLoggedIn) return false;

        if (name == "btn_Home")     { ShowMainScreen("Home"); return true; }
        if (name == "btn_Map")      { ShowMainScreen("Map"); return true; }
        if (name == "btn_Contacts") { ShowMainScreen("Contacts"); return true; }
        if (name == "btn_Files")    { ShowMainScreen("Files"); return true; }
        if (name == "btn_Notes")    { ShowMainScreen("Notes"); return true; }
        if (name == "btn_Profile")  { ShowMainScreen("Profile"); return true; }

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
            ShowSystemMessage("Registration successful!");
            ShowAuthScreen("Login");
        }
        else
        {
            ShowSystemMessage("Registration failed.");
        }
    }

    void RPC_LoginResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;
        Param2<bool, string> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("Login successful!");
            m_IsLoggedIn = true;
            s_IsLoggedInThisSession = true;
            ShowMainScreen(data.param2 != "" ? data.param2 : "Home");
        }
        else
        {
            ShowSystemMessage("Login failed.");
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
    }
}