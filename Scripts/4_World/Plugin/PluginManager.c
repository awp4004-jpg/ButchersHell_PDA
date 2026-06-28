modded class PluginManager
{
    override void Init()
    {
        // Client plugins
        if (GetGame().IsClient())
        {
            // We can add client-only plugins here later
        }

        // Server plugins
        if (GetGame().IsServer())
        {
            RegisterPlugin("PDA_AccountPlugin", true, true);
        }

        super.Init();
    }
    
    void RPC_RegisterAccount(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        string username = data.param1;
        string password = data.param2;

        PDA_AccountPlugin plugin;
        Class.CastTo(plugin, GetPlugin(PDA_AccountPlugin));

        if (plugin)
        {
            bool success = plugin.RegisterAccount(username, password);
            GetRPCManager().SendRPC("PDA", "RPC_RegisterResponse", new Param1<bool>(success), true, sender);
        }
    }

    void RPC_LoginAccount(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type != CallType.Server) return;

        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        string username = data.param1;
        string password = data.param2;

        PDA_AccountPlugin plugin;
        Class.CastTo(plugin, GetPlugin(PDA_AccountPlugin));

        if (plugin)
        {
            bool success = plugin.LoginAccount(username, password);
            GetRPCManager().SendRPC("PDA", "RPC_LoginResponse", new Param1<bool>(success), true, sender);
        }
    }
}