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

        // Register all RPCs
        GetRPCManager().AddRPC("PDA", "RPC_RegisterAccount", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_LoginAccount", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_SetPin", this, SingleplayerExecutionType.Server);

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
            Print("[PDA] Loaded " + m_Accounts.Count() + " accounts from JSON");
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
        newAccount.pin = "";                    // PIN is empty until user sets it

        m_Accounts.Insert(username, newAccount);
        SaveAccounts();

        Print("[PDA] New account registered: " + username);
        return true;
    }

    bool LoginAccount(string username, string password)
    {
        if (!m_Accounts.Contains(username)) return false;
        return m_Accounts.Get(username).password == password;
    }

    bool SetPin(string username, string newPin)
    {
        if (!m_Accounts.Contains(username)) return false;

        m_Accounts.Get(username).pin = newPin;
        SaveAccounts();

        Print("[PDA] PIN updated for user: " + username);
        return true;
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

        bool success = LoginAccount(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_LoginResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_SetPin(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        bool success = SetPin(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_SetPinResponse", new Param1<bool>(success), true, sender);
    }
}

// ==================== Account Data Class ====================
class PDA_Account
{
    string username;
    string password;
    string pin = "";
}