class ActionTurnOffPDA: ActionSingleUseBase
{
    void ActionTurnOffPDA()
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
        return "Turn Off PDA";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!item || !item.IsKindOf("PDA_Base")) return false;

        PDA_Base pda = PDA_Base.Cast(item);
        if (!pda) return false;

        return pda.IsOn(); // Only show if PDA is on
    }

    override void OnExecuteClient(ActionData action_data)
    {
        super.OnExecuteClient(action_data);

        ItemBase item = action_data.m_MainItem;
        if (!item) return;

        PDA_Base pda = PDA_Base.Cast(item);
        if (pda) pda.TurnOff();
    }
}