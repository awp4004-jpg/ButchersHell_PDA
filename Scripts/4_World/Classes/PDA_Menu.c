class PDA_Menu extends UIScriptedMenu
{
protected string m_LastOpenedApp = "Home";
    protected bool m_IsLoggedIn = false;
    protected bool m_IsRegistered = false;

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

            // Status bar widgets
            m_txtTime    = TextWidget.Cast(root.FindAnyWidget("txt_Time"));
            m_txtBattery = TextWidget.Cast(root.FindAnyWidget("txt_Battery"));
            m_txtSignal  = TextWidget.Cast(root.FindAnyWidget("txt_Signal"));
            m_txtDate    = TextWidget.Cast(root.FindAnyWidget("txt_Date"));
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

            m_lgnName     = EditBoxWidget.Cast(root.FindAnyWidget("lgn_Name_txt"));
            m_lgnPassword = EditBoxWidget.Cast(root.FindAnyWidget("lgn_Password_txt"));

            // Register RPC responses
            GetRPCManager().AddRPC("PDA", "RPC_RegisterResponse", this, SingleplayerExecutionType.Client);
            GetRPCManager().AddRPC("PDA", "RPC_LoginResponse", this, SingleplayerExecutionType.Client);

            // Load if player is registered (this stays saved)
            string registered;
            if (GetGame().GetProfileString("PDA_IsRegistered", registered))
            {
                m_IsRegistered = (registered == "true");
            }

            // Always reset login state on new connection
            m_IsLoggedIn = false;

            // Decide starting screen
            if (!m_IsRegistered)
            {
                ShowAuthScreen("Register");
            }
            else
            {
                ShowAuthScreen("Login");   // Always start with Login after reconnect
            }
        }
        
        return root;
    }

    void ShowAuthScreen(string screenName)
    {
        LockBottomButtons(true);

        if (m_screenRegister) m_screenRegister.Show(false);
        if (m_screenLogin)    m_screenLogin.Show(false);
        if (m_screenPin)      m_screenPin.Show(false);

        if (screenName == "Register" && m_screenRegister)
            m_screenRegister.Show(true);
        else if (screenName == "Login" && m_screenLogin)
            m_screenLogin.Show(true);
        else if (screenName == "Pin" && m_screenPin)
            m_screenPin.Show(true);
    }

    void ShowMainScreen(string screenName)
    {
        LockBottomButtons(false);
        m_IsLoggedIn = true;                    // Only for current session

        // Hide auth screens
        if (m_screenRegister) m_screenRegister.Show(false);
        if (m_screenLogin)    m_screenLogin.Show(false);
        if (m_screenPin)      m_screenPin.Show(false);

        m_LastOpenedApp = screenName;
        GetGame().SetProfileString("LastPDA_Screen", screenName);

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

    override bool OnFocus(Widget w, int x, int y)
    {
        EditBoxWidget edit = EditBoxWidget.Cast(w);
        if (!edit) return false;

        string currentText = edit.GetText();

        // List of placeholder texts you want to clear
        if (currentText == "Login Name..." || currentText == "**********" || currentText == "Conctact Name..." || currentText == "Text..." || currentText == "Category Name...." || currentText == "Note Name..." || currentText == "*******")
        {
            edit.SetText("");
        }

        return false;
    }

    // ==================== System Message ====================

    void ShowSystemMessage(string message)
    {
        if (!m_SystemMessage) return;

        m_SystemMessage.SetText(message);

        // Clear the message after 5 seconds
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ClearSystemMessage, 5000, false);
    }

    void ClearSystemMessage()
    {
        if (m_SystemMessage)
        {
            m_SystemMessage.SetText("");
        }
    }

    // ==================== Updated OnClick ====================
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (button != MouseState.LEFT) return false;

        string name = w.GetName();

        if (name == "btn_Close") { Close(); return true; }

        // Switch between Register <-> Login
        if (name == "rgr_Login_btn")
        {
            ShowAuthScreen("Login");
            return true;
        }

        if (name == "lgn_Register_btn")
        {
            ShowAuthScreen("Register");
            return true;
        }

        // ==================== REGISTER ====================
        if (name == "rgr_Register_btn")
        {
            if (!m_rgrName || !m_rgrPassword || !m_rgrPasswordAgain) return true;

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

        // ==================== LOGIN ====================
        if (name == "lgn_Login_btn")
        {
            if (!m_lgnName || !m_lgnPassword) return true;

            string loginUser = m_lgnName.GetText();
            string loginPass = m_lgnPassword.GetText();

            GetRPCManager().SendRPC("PDA", "RPC_LoginAccount", new Param2<string, string>(loginUser, loginPass), true);
            return true;
        }

        // Main screen buttons
        if (!m_IsLoggedIn) return false;

        if (name == "btn_Home")     { ShowMainScreen("Home"); return true; }
        if (name == "btn_Map")      { ShowMainScreen("Map"); return true; }
        if (name == "btn_Contacts") { ShowMainScreen("Contacts"); return true; }
        if (name == "btn_Files")    { ShowMainScreen("Files"); return true; }
        if (name == "btn_Notes")    { ShowMainScreen("Notes"); return true; }
        if (name == "btn_Profile")  { ShowMainScreen("Profile"); return true; }

        return false;
    }

    // ==================== RPC Responses ====================

    void RPC_RegisterResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;

        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("Registration successful!");   // ← Changed
            ShowAuthScreen("Login");
        }
        else
        {
            ShowSystemMessage("Registration failed."); // ← Changed
        }
    }

    void RPC_LoginResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Client) return;

        Param1<bool> data;
        if (!ctx.Read(data)) return;

        if (data.param1)
        {
            ShowSystemMessage("Login successful!");     // ← Changed
            m_IsLoggedIn = true;
            ShowMainScreen(m_LastOpenedApp);
        }
        else
        {
            ShowSystemMessage("Login failed."); // ← Changed
        }
    }

    override bool OnKeyPress(Widget w, int x, int y, int key)
    {
        if (key == KeyCode.KC_ESCAPE)
        {
            Close();
            return true;
        }
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

        if (m_txtTime)
        {
            GetGame().GetWorld().GetDate(year, month, day, hour, minute);
            m_txtTime.SetText(string.Format("%1:%2", hour.ToStringLen(2), minute.ToStringLen(2)));
        }

        if (m_txtDate)
        {
            GetGame().GetWorld().GetDate(year, month, day, hour, minute);
            m_txtDate.SetText(string.Format("%1.%2.%3", year, month.ToStringLen(2), day.ToStringLen(2)));
        }

        if (m_txtBattery)
        {
            PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
            if (player)
            {
                ItemBase itemInHands = player.GetItemInHands();
                if (itemInHands)
                {
                    EntityAI battery = itemInHands.FindAttachmentBySlotName("BatteryD");
                    if (battery && battery.HasEnergyManager())
                    {
                        float energy01 = battery.GetCompEM().GetEnergy0To1();
                        int percent = Math.Round(energy01 * 100);
                        m_txtBattery.SetText("Battery: " + percent.ToString() + "%");
                    }
                    else
                    {
                        m_txtBattery.SetText("Battery: --% (no battery)");
                    }
                }
                else
                {
                    m_txtBattery.SetText("Battery: --%");
                }
            }
        }

        if (m_txtSignal)
        {
            m_txtSignal.SetText("Signal: ●●●○○");
        }
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

        if (layoutRoot)
            SetFocus(layoutRoot);
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