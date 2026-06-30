class PDA_Base extends Inventory_Base
{
    string m_PDAID;

    void PDA_Base()
    {
        m_PDAID = "";
    }

    void GeneratePDAID()
    {
        if (m_PDAID == "")
        {
            m_PDAID = (Math.RandomInt(100000, 999999)).ToString();
            Print("[PDA] New PDA ID generated: " + m_PDAID);
        }
    }

    string GetPDAID()
    {
        return m_PDAID;
    }

    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionOpenPDA);
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return true;
    }
}