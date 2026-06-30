class PDA_AccountPlugin extends PluginBase
{
    ref map<string, ref PDA_Account> m_Accounts;
    ref map<string, ref PDA_PlayerData> m_PlayerData;

    void PDA_AccountPlugin()
    {
        m_Accounts = new map<string, ref PDA_Account>();
        m_PlayerData = new map<string, ref PDA_PlayerData>();
    }

    override void OnInit()
    {
        super.OnInit();

        // Register all RPCs
        GetRPCManager().AddRPC("PDA", "RPC_RegisterAccount", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_LoginAccount", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_SetPin", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_VerifyPin", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_ChangePin", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_SetPinEnabled", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_SetDisplayName", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_AddContact", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_RemoveContact", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_GetContacts", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_SendMessage", this, SingleplayerExecutionType.Server);
        GetRPCManager().AddRPC("PDA", "RPC_GetMessages", this, SingleplayerExecutionType.Server);

        LoadAccounts();
        LoadPDAData();
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
    bool RegisterAccount(string username, string password, string steamID = "")
    {
        if (username == "" || password == "" || m_Accounts.Contains(username))
            return false;

        ref PDA_Account newAccount = new PDA_Account();
        newAccount.username = username;
        newAccount.password = password;
        newAccount.steamID = steamID;
        newAccount.pin = "";
        newAccount.pinEnabled = false;

        m_Accounts.Insert(username, newAccount);
        SaveAccounts();

        Print("[PDA] New account registered: " + username + " | SteamID: " + steamID);
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
        return true;
    }

    bool VerifyPin(string username, string enteredPin)
    {
        if (!m_Accounts.Contains(username)) return false;

        ref PDA_Account account = m_Accounts.Get(username);

        if (!account.pinEnabled) return true; // If PIN is disabled, always allow

        return (account.pin == enteredPin);
    }

    bool ChangePin(string username, string oldPin, string newPin)
    {
        if (!m_Accounts.Contains(username)) return false;

        ref PDA_Account account = m_Accounts.Get(username);

        if (account.pin != oldPin) return false; // Wrong old PIN

        account.pin = newPin;
        SaveAccounts();
        return true;
    }

    bool SetPinEnabled(string username, bool enabled)
    {
        if (!m_Accounts.Contains(username)) return false;

        m_Accounts.Get(username).pinEnabled = enabled;
        SaveAccounts();
        return true;
    }

    bool IsPinEnabled(string username)
    {
        if (!m_Accounts.Contains(username)) return false;
        return m_Accounts.Get(username).pinEnabled;
    }

    bool SetDisplayName(string username, string newName)
    {
        if (!m_Accounts.Contains(username) || newName == "") return false;

        m_Accounts.Get(username).displayName = newName;
        SaveAccounts();
        return true;
    }

    // ==================== PDA DATA (Contacts + Messages) ====================
    void LoadPDAData()
    {
        string path = "$profile:\\PDA\\pda_data.json";

        if (FileExist(path))
        {
            JsonFileLoader<ref map<string, ref PDA_PlayerData>>.JsonLoadFile(path, m_PlayerData);
            Print("[PDA] Loaded PDA data for " + m_PlayerData.Count() + " players");
        }
        else
        {
            Print("[PDA] No pda_data.json found. Will be created when data is saved.");
        }
    }

    void SavePDAData()
    {
        string path = "$profile:\\PDA\\pda_data.json";
        JsonFileLoader<ref map<string, ref PDA_PlayerData>>.JsonSaveFile(path, m_PlayerData);
    }

    // Get or create player data
    PDA_PlayerData GetOrCreatePlayerData(string username)
    {
        if (!m_PlayerData.Contains(username))
        {
            ref PDA_PlayerData newData = new PDA_PlayerData();
            newData.username = username;
            newData.messages = new map<string, ref array<ref ChatMessage>>();
            m_PlayerData.Insert(username, newData);
        }
        return m_PlayerData.Get(username);
    }

    // ==================== CONTACTS ====================
    bool AddContact(string username, string contactName)
    {
        if (username == "" || contactName == "" || username == contactName)
            return false;

        ref PDA_PlayerData data = GetOrCreatePlayerData(username);

        // Use Find() instead of Contains() for arrays
        if (data.contacts.Find(contactName) == -1)
        {
            data.contacts.Insert(contactName);
            SavePDAData();
            return true;
        }
        return false;
    }

    bool RemoveContact(string username, string contactName)
    {
        if (!m_PlayerData.Contains(username))
            return false;

        ref PDA_PlayerData data = m_PlayerData.Get(username);

        int index = data.contacts.Find(contactName);
        if (index != -1)
        {
            data.contacts.Remove(index);
            SavePDAData();
            return true;
        }
        return false;
    }

    array<string> GetContacts(string username)
    {
        if (!m_PlayerData.Contains(username))
            return new array<string>();

        return m_PlayerData.Get(username).contacts;
    }

    // ==================== SEND MESSAGE ====================
    bool SendMessage(string fromUsername, string toUsername, string messageText)
    {
        if (fromUsername == "" || toUsername == "" || messageText == "" || fromUsername == toUsername)
            return false;

        ref PDA_PlayerData senderData = GetOrCreatePlayerData(fromUsername);
        ref PDA_PlayerData receiverData = GetOrCreatePlayerData(toUsername);

        ref ChatMessage newMessage = new ChatMessage();
        newMessage.from = fromUsername;
        newMessage.to = toUsername;
        newMessage.message = messageText;

        // Proper timestamp
        int year, month, day, hour, minute;
        GetGame().GetWorld().GetDate(year, month, day, hour, minute);
        newMessage.timestamp = string.Format("%1.%2.%3 %4:%5", year, month, day, hour, minute);

        newMessage.read = false;

        // Add to sender
        if (!senderData.messages.Contains(toUsername))
            senderData.messages.Insert(toUsername, new array<ref ChatMessage>());

        senderData.messages.Get(toUsername).Insert(newMessage);

        // Add to receiver
        if (!receiverData.messages.Contains(fromUsername))
            receiverData.messages.Insert(fromUsername, new array<ref ChatMessage>());

        receiverData.messages.Get(fromUsername).Insert(newMessage);

        SavePDAData();
        return true;
    }

    array<ref ChatMessage> GetMessagesWith(string username, string withUsername)
    {
        if (!m_PlayerData.Contains(username))
            return new array<ref ChatMessage>();

        ref PDA_PlayerData data = m_PlayerData.Get(username);

        if (!data.messages.Contains(withUsername))
            return new array<ref ChatMessage>();

        return data.messages.Get(withUsername);
    }

    // ==================== RPC Handlers ====================
    void RPC_RegisterAccount(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        string username = data.param1;
        string password = data.param2;
        string steamID = sender.GetPlainId();

        bool success = RegisterAccount(username, password, steamID);   // Pass steamID
        GetRPCManager().SendRPC("PDA", "RPC_RegisterResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_LoginAccount(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        string username = data.param1;
        string password = data.param2;

        bool passwordCorrect = LoginAccount(username, password);

        if (passwordCorrect)
        {
            bool requiresPin = IsPinEnabled(username);
            string displayName = m_Accounts.Get(username).displayName;   // NEW

            GetRPCManager().SendRPC("PDA", "RPC_LoginResponse", 
                new Param3<bool, bool, string>(true, requiresPin, displayName), true, sender);
        }
        else
        {
            GetRPCManager().SendRPC("PDA", "RPC_LoginResponse", 
                new Param3<bool, bool, string>(false, false, ""), true, sender);
        }
    }

    void RPC_SetPin(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        bool success = SetPin(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_SetPinResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_VerifyPin(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        bool success = VerifyPin(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_VerifyPinResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_ChangePin(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param3<string, string, string> data;
        if (!ctx.Read(data)) return;

        bool success = ChangePin(data.param1, data.param2, data.param3);
        GetRPCManager().SendRPC("PDA", "RPC_ChangePinResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_SetPinEnabled(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, bool> data;
        if (!ctx.Read(data)) return;

        bool success = SetPinEnabled(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_SetPinEnabledResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_SetDisplayName(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        bool success = SetDisplayName(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_SetDisplayNameResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_AddContact(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        bool success = AddContact(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_AddContactResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_RemoveContact(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        bool success = RemoveContact(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_RemoveContactResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_GetContacts(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param1<string> data;
        if (!ctx.Read(data)) return;

        array<string> contacts = GetContacts(data.param1);
        GetRPCManager().SendRPC("PDA", "RPC_GetContactsResponse", new Param1<ref array<string>>(contacts), true, sender);
    }

    void RPC_SendMessage(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param3<string, string, string> data;
        if (!ctx.Read(data)) return;

        bool success = SendMessage(data.param1, data.param2, data.param3);

        // Optional: Send response back to sender
        GetRPCManager().SendRPC("PDA", "RPC_SendMessageResponse", new Param1<bool>(success), true, sender);
    }

    void RPC_GetMessages(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server || !sender) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        array<ref ChatMessage> messages = GetMessagesWith(data.param1, data.param2);
        GetRPCManager().SendRPC("PDA", "RPC_GetMessagesResponse", new Param1<ref array<ref ChatMessage>>(messages), true, sender);
    }
}

// ==================== Account Data Class ====================
class PDA_Account
{
    string username;
    string password;
    string pin = "";
    string steamID = "";
    string displayName = "";
    bool pinEnabled = false;
}

// ==================== PDA Player Data (Contacts + Messages) ====================
class PDA_PlayerData
{
    string username;
    ref array<string> contacts = new array<string>();
    ref map<string, ref array<ref ChatMessage>> messages = new map<string, ref array<ref ChatMessage>>();
}

class ChatMessage
{
    string from;
    string to;
    string message;
    string timestamp;
    bool read = false;
}