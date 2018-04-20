/*
 * script to read the trees of L1Ntuple file in a ROOT session
 * usage : root [1].x Utilities/readL1NtupleTrees.h
 */

TTree* tl1Evt = 0;
TTree* tmuonEmu = 0;
TTree* tcalotowEmu = 0;
TTree* tupgradeEmu = 0;
TTree* tgtEmu = 0;

void readL1NtupleTrees()
{
    tl1Evt = (TTree*)gFile->Get("l1EventTree/L1EventTree");
    tmuonEmu = (TTree*)gFile->Get("l1UpgradeTfMuonEmuTree/L1UpgradeTfMuonTree");
    tcalotowEmu = (TTree*)gFile->Get("l1CaloTowerEmuTree/L1CaloTowerTree");
    tupgradeEmu = (TTree*)gFile->Get("l1UpgradeEmuTree/L1UpgradeTree");
    tgtEmu = (TTree*)gFile->Get("l1uGTEmuTree/L1uGTTree");

    if (tl1Evt != 0)
        std::cout << "Tree* tl1Evt = l1EventTree/L1EventTree" << std::endl;
    if (tmuonEmu != 0)
        std::cout << "Tree* tmuonEmu = l1UpgradeTfMuonEmuTree/l1UpgradeTfMuonEmuTree" << std::endl;
    if (tcalotowEmu != 0)
        std::cout << "Tree* tcalotowEmu = l1CaloTowerEmuTree/l1CaloTowerEmuTree" << std::endl;
    if (tupgradeEmu != 0)
        std::cout << "Tree* tupgradeEmu = l1UpgradeEmuTree/L1UpgradeTree" << std::endl;
    if (tgtEmu != 0)
        std::cout << "Tree* tgtEmu = l1uGTEmuTree/l1uGTEmuTree" << std::endl;
}
