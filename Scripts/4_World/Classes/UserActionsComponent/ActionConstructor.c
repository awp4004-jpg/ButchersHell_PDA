modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(ActionOpenPDA);
        actions.Insert(ActionTurnOnPDA);
        actions.Insert(ActionTurnOffPDA);        
    }
}