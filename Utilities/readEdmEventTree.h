/*
 * script to read the branches of Event tree in EDM file
 * usage : root [1].x Utilities/readEdmEventTree.h
 */

TTree* t = 0;

void readEdmEventTree()
{
    t = (TTree*)gFile->Get("Events");

    if (t == 0) return;

    std::cout << "Tree* t = Events" << std::endl;

    // event info
    t->SetAlias("run","EventAuxiliary.id_.run_");
    t->SetAlias("lumis","EventAuxiliary.id_.luminosityBlock_");
    t->SetAlias("event","EventAuxiliary.id_.event_");
    //t->Scan("run:lumis:event")
    std::cout << "t->Scan(\"run:lumis:event\")" << std::endl;

    bool readGenParticles = true;
    std::string pathGenParticles = "recoGenParticles_genParticles__SIM";

    if (readGenParticles) {

        t->SetAlias("ngen", Form("%s.@obj.size()", pathGenParticles.c_str()));
        t->SetAlias("genpt", Form("%s.obj.pt()", pathGenParticles.c_str()));
        t->SetAlias("geneta", Form("%s.obj.eta()", pathGenParticles.c_str()));
        t->SetAlias("genphi", Form("%s.obj.phi()", pathGenParticles.c_str()));

        //t->Scan("ngen:genpt:geneta:genphi")
        std::cout << "t->Scan(\"ngen:genpt:geneta:genphi\")" << std::endl;

        // t->Scan("ngen:Length$(genpt)") to make sure ngen is read correctly
    }
}
