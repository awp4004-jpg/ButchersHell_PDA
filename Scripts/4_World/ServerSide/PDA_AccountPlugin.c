class PDA_AccountPlugin extends PluginBase
{
    ref map<string, ref PDA_Account> m_Accounts;

    void PDA_AccountPlugin()
    {
        m_Accounts = new map<string, ref PDA_Account>();
    }

    override void OnInit()
    {
        super.OnInit();

        // Register RPCs
        GetRPCManager().AddRPC("PDA", "RPC_RegisterAccount", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_LoginAccount", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_SaveLastScreen", this, SingleplayerExecutionType.Server);

        LoadAccounts();
        Print("[PDA] PDA_AccountPlugin initialized on server");
    }

    // ==================== JSON Save / Load ====================
    void LoadAccounts()
    {
        string path = "$profile:\\PDA\\accounts.json";

        if (FileExist(path))
        {
            JsonFileLoader<ref map<string, ref PDA_Account>>.JsonLoadFile(path, m_Accounts);
            Print("[PDA] Loaded " + m_Accounts.Count() + " accounts");
        }
        else
        {
            Print("[PDA] No accounts file found. Creating directory...");
            MakeDirectory("$profile:\\PDA\\");
        }
    }

    void SaveAccounts()
    {
        string path = "$profile:\\PDA\\accounts.json";
        JsonFileLoader<ref map<string, ref PDA_Account>>.JsonSaveFile(path, m_Accounts);
    }

    // ==================== Account Logic ====================
    bool RegisterAccount(string username, string password)
    {
        if (username == "" || password == "" || m_Accounts.Contains(username))
            return false;

        ref PDA_Account newAccount = new PDA_Account();
        newAccount.username = username;
        newAccount.password = password;
        newAccount.lastScreen = "Home";

        m_Accounts.Insert(username, newAccount);
        SaveAccounts();

        Print("[PDA] New account registered: " + username);
        return true;
    }

    bool LoginAccount(string username, string password)
    {
        if (!m_Accounts.Contains(username))
            return false;

        return m_Accounts.Get(username).password == password;
    }

    void SaveLastScreen(string username, string screen)
    {
        if (!m_Accounts.Contains(username)) return;

        m_Accounts.Get(username).lastScreen = screen;
        SaveAccounts();
    }

    string GetLastScreen(string username)
    {
        if (!m_Accounts.Contains(username)) return "Home";
        return m_Accounts.Get(username).lastScreen;
    }

    // ==================== RPC Handlers ====================
    void RPC_RegisterAccount(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        bool success = RegisterAccount(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_RegisterResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_LoginAccount(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        string username = data.param1;
        string password = data.param2;

        bool success = LoginAccount(username, password);

        if (success)
        {
            string lastScreen = GetLastScreen(username);
            GetRPCManager().SendRPC("PDA", "RPC_LoginResponse", new Param2<bool, string>(true, lastScreen), true, sender);
        }
        else
        {
            GetRPCManager().SendRPC("PDA", "RPC_LoginResponse", new Param2<bool, string>(false, ""), true, sender);
        }
    }

    void RPC_SaveLastScreen(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        SaveLastScreen(data.param1, data.param2);
    }
}

// ==================== Account Data Class ====================
class PDA_Account
{
    string username;
    string password;
    string lastScreen = "Home";
}