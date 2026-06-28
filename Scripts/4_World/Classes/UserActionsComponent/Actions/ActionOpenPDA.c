class ActionOpenPDA: ActionSingleUseBase
{
    void ActionOpenPDA()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
    }

    override void CreateConditionComponents() 
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTNone;
    }

    override bool HasTarget()
    {
        return false;
    }

    override string GetText()
    {
        return "Open PDA";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        return true; // Change this later to check for your PDA item
    }

    override void OnStartClient(ActionData action_data)
    {
        super.OnStartClient(action_data);
        GetGame().GetUIManager().EnterScriptedMenu(PDA_MENU.PDA, null);
    }
}