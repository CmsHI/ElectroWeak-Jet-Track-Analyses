/*
 * script to read the trees of a HiForest file in a ROOT session
 * usage : root [1].x Utilities/readHiForestTrees.h
 */

TTree* thlt = 0;
TTree* tl1obj = 0;
TTree* teg = 0;
TTree* tegGED = 0;
TTree* tskim = 0;
TTree* tevt = 0;
TTree* tGen = 0;
TTree* t3 = 0;
TTree* t3calo = 0;
TTree* t3cs = 0;
TTree* t3pp = 0;
TTree* t4 = 0;
TTree* t4calo = 0;
TTree* t4pp = 0;
TTree* t4cs = 0;
TTree* trho = 0;
TTree* tpf = 0;
TTree* tpfcs = 0;
TTree* ttrack = 0;
TTree* ttrackpp = 0;
TTree* tEE = 0;
TTree* tEB = 0;
TTree* ttower = 0;
TTree* tBC = 0;
TNtuple* ntEvent = 0;

void readHiForestTrees()
{
    thlt = (TTree*)gFile->Get("hltanalysis/HltTree");
    tl1obj = (TTree*)gFile->Get("l1object/L1UpgradeFlatTree");
    teg = (TTree*)gFile->Get("ggHiNtuplizer/EventTree");
    tegGED = (TTree*)gFile->Get("ggHiNtuplizerGED/EventTree");
    tskim = (TTree*)gFile->Get("skimanalysis/HltTree");
    tevt = (TTree*)gFile->Get("hiEvtAnalyzer/HiTree");
    tGen = (TTree*)gFile->Get("HiGenParticleAna/hi");
    t3 = (TTree*)gFile->Get("akPu3PFJetAnalyzer/t");
    t3calo = (TTree*)gFile->Get("akPu3CaloJetAnalyzer/t");
    t3pp = (TTree*)gFile->Get("ak3PFJetAnalyzer/t");
    t3cs = (TTree*)gFile->Get("akCs3PFJetAnalyzer/t");
    t4 = (TTree*)gFile->Get("akPu4PFJetAnalyzer/t");
    t4calo = (TTree*)gFile->Get("akPu4CaloJetAnalyzer/t");
    t4pp = (TTree*)gFile->Get("ak4PFJetAnalyzer/t");
    t4cs = (TTree*)gFile->Get("akCs4PFJetAnalyzer/t");
    trho = (TTree*)gFile->Get("hiFJRhoAnalyzer/t");
    tpf = (TTree*)gFile->Get("pfcandAnalyzer/pfTree");
    tpfcs = (TTree*)gFile->Get("pfcandAnalyzerCS/pfTree");
    ttrack = (TTree*)gFile->Get("anaTrack/trackTree");
    ttrackpp = (TTree*)gFile->Get("ppTrack/trackTree");
    tEE = (TTree*)gFile->Get("rechitanalyzer/ee");
    tEB = (TTree*)gFile->Get("rechitanalyzer/eb");
    ttower = (TTree*)gFile->Get("rechitanalyzer/tower");
    tBC = (TTree*)gFile->Get("rechitanalyzer/bc");
    ntEvent = (TNtuple*)gFile->Get("rechitanalyzer/ntEvent");

    if (thlt != 0)
        std::cout << "Tree* thlt = hltanalysis/HltTree" << std::endl;
    if (tl1obj != 0)
        std::cout << "Tree* tl1obj = l1object/L1UpgradeFlatTree" << std::endl;
    if (teg != 0)
        std::cout << "Tree* teg = ggHiNtuplizer/EventTree" << std::endl;
    if (tegGED != 0)
        std::cout << "Tree* tegGED = ggHiNtuplizerGED/EventTree" << std::endl;
    if (tskim != 0)
        std::cout << "Tree* tskim = skimanalysis/HltTree" << std::endl;
    if (tevt != 0)
        std::cout << "Tree* tevt = hiEvtAnalyzer/HiTree" << std::endl;
    if (tGen != 0)
        std::cout << "Tree* tGen = HiGenParticleAna/hi" << std::endl;
    if (t3 != 0)
        std::cout << "Tree* t3 = akPu3PFJetAnalyzer/t" << std::endl;
    if (t3calo != 0)
        std::cout << "Tree* t3calo = akPu3CaloJetAnalyzer/t" << std::endl;
    if (t3pp != 0)
        std::cout << "Tree* t3pp = ak3PFJetAnalyzer/t" << std::endl;
    if (t3cs != 0)
        std::cout << "Tree* t3cs = akCs3PFJetAnalyzer/t" << std::endl;
    if (t4 != 0)
        std::cout << "Tree* t4 = akPu4PFJetAnalyzer/t" << std::endl;
    if (t4calo != 0)
        std::cout << "Tree* t4calo = akPu4CaloJetAnalyzer/t" << std::endl;
    if (t4pp != 0)
        std::cout << "Tree* t4pp = ak4PFJetAnalyzer/t" << std::endl;
    if (t4cs != 0)
        std::cout << "Tree* t4cs = akCs4PFJetAnalyzer/t" << std::endl;
    if (trho != 0)
        std::cout << "Tree* trho = hiFJRhoAnalyzer/t" << std::endl;
    if (tpf != 0)
        std::cout << "Tree* tpf = pfcandAnalyzer/pfTree" << std::endl;
    if (tpfcs != 0)
        std::cout << "Tree* tpfcs = pfcandAnalyzerCS/pfTree" << std::endl;
    if (ttrack != 0)
        std::cout << "Tree* ttrack = anaTrack/trackTree" << std::endl;
    if (ttrackpp != 0)
        std::cout << "Tree* ttrackpp = ppTrack/trackTree" << std::endl;
    if (tEE != 0)
        std::cout << "Tree* tEE = rechitanalyzer/ee" << std::endl;
    if (tEB != 0)
        std::cout << "Tree* tEB = rechitanalyzer/eb" << std::endl;
    if (ttower != 0)
        std::cout << "Tree* ttower = rechitanalyzer/tower" << std::endl;
    if (tBC != 0)
        std::cout << "Tree* tBC = rechitanalyzer/bc" << std::endl;
    if (ntEvent != 0)
        std::cout << "TNtuple* ntEvent = rechitanalyzer/ntEvent" << std::endl;

    if (teg != 0) {
        teg->AddFriend(tskim);
        teg->AddFriend(tevt);
        teg->AddFriend(t3);
        teg->AddFriend(t3pp);
    }

    if (tegGED != 0) {
        tegGED->AddFriend(tskim);
        tegGED->AddFriend(tevt);
        tegGED->AddFriend(t3);
        tegGED->AddFriend(t3pp);
    }
}
