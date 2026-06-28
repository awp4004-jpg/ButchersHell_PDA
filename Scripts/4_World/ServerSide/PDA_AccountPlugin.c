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

        LoadAccounts();
        Print("[PDA] PDA_AccountPlugin initialized on server");
    }

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
            Print("[PDA] No accounts file found. A new one will be created.");
            MakeDirectory("$profile:\\PDA\\");
        }
    }

    void SaveAccounts()
    {
        string path = "$profile:\\PDA\\accounts.json";
        JsonFileLoader<ref map<string, ref PDA_Account>>.JsonSaveFile(path, m_Accounts);
    }

    // ==================== Register ====================
    bool RegisterAccount(string username, string password)
    {
        if (m_Accounts.Contains(username))
            return false; // Username already exists

        ref PDA_Account newAccount = new PDA_Account();
        newAccount.username = username;
        newAccount.password = password;

        m_Accounts.Insert(username, newAccount);
        SaveAccounts();

        Print("[PDA] New account created: " + username);
        return true;
    }

    // ==================== Login ====================
    bool LoginAccount(string username, string password)
    {
        if (!m_Accounts.Contains(username))
            return false;

        ref PDA_Account account = m_Accounts.Get(username);
        return (account.password == password);
    }

    // ==================== RPC Handlers ====================

    void RPC_RegisterAccount(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        string username = data.param1;
        string password = data.param2;

        bool success = RegisterAccount(username, password);
        GetRPCManager().SendRPC("PDA", "RPC_RegisterResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_LoginAccount(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        string username = data.param1;
        string password = data.param2;

        bool success = LoginAccount(username, password);
        GetRPCManager().SendRPC("PDA", "RPC_LoginResponse", new Param1<bool>(success), true, sender);
    }
}

class PDA_Account
{
    string username;
    string password;
}