#ifndef TREEHEADERS_TRACKJETTREE_H_
#define TREEHEADERS_TRACKJETTREE_H_

#include <TTree.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TVector3.h>

#include <vector>

#include "../Plotting/commonUtility.h"
#include "JetTree.h"
#include "trackTree.h"

class TrackJet {
public :
    TrackJet(){
        coneRange = 0.3;

        trackIdx = 0;
        jetIdx = 0;
        z = 0;
        ksi = 0;
        zt = 0;
        angle = 0;
        deta = 0;
        dphi = 0;
        dR = 0;
    }
    ~TrackJet(){};
    void resetConeRange() { coneRange = 0.3 ; }
    void setupTrackJetTree(TTree *t);
    void branchTrackJetTree(TTree *t);
    void clearTrackJetPairs();
    void makeTrackJetPair(Tracks &tTracks, Jets &tJets, int jetIndex);
    void makeTrackJetPair(Tracks &tTracks, int trackIndex, Jets &tJets, int jetIndex);
    void makeTrackJetPairs(Tracks &tTracks, Jets &tJets);

    float coneRange;

    // Declaration of leaf types
     std::vector<int>     *trackIdx;
     std::vector<int>     *jetIdx;
     std::vector<float>   *z;       // z = (momentum of track projected to jet axis) / jet momentum
     std::vector<float>   *ksi;     // ksi = log (1/z)
     std::vector<float>   *zt;      // zt = (track pt) / (jet pt)
     std::vector<float>   *angle;   // angle between the track and jet
     std::vector<float>   *deta;
     std::vector<float>   *dphi;
     std::vector<float>   *dR;

     // List of branches
     TBranch        *b_trackIdx;   //!
     TBranch        *b_jetIdx;   //!
     TBranch        *b_z;   //!
     TBranch        *b_ksi;   //!
     TBranch        *b_zt;   //!
     TBranch        *b_angle;   //!
     TBranch        *b_deta;   //!
     TBranch        *b_dphi;   //!
     TBranch        *b_dR;   //!

     // list of objects to be used when creating a trackJet Tree
     std::vector<int>     trackIdx_out;
     std::vector<int>     jetIdx_out;
     std::vector<float>   z_out;
     std::vector<float>   ksi_out;
     std::vector<float>   zt_out;
     std::vector<float>   angle_out;
     std::vector<float>   deta_out;
     std::vector<float>   dphi_out;
     std::vector<float>   dR_out;

};

void TrackJet::setupTrackJetTree(TTree *t)
{
    if (t->GetBranch("trackIdx"))  t->SetBranchAddress("trackIdx", &trackIdx, &b_trackIdx);
    if (t->GetBranch("jetIdx"))  t->SetBranchAddress("jetIdx", &jetIdx, &b_jetIdx);
    if (t->GetBranch("z"))  t->SetBranchAddress("z", &z, &b_z);
    if (t->GetBranch("ksi"))  t->SetBranchAddress("ksi", &ksi, &b_ksi);
    if (t->GetBranch("zt"))  t->SetBranchAddress("zt", &zt, &b_zt);
    if (t->GetBranch("angle"))  t->SetBranchAddress("angle", &angle, &b_angle);
    if (t->GetBranch("deta"))  t->SetBranchAddress("deta", &deta, &b_deta);
    if (t->GetBranch("dphi"))  t->SetBranchAddress("dphi", &dphi, &b_dphi);
    if (t->GetBranch("dR"))  t->SetBranchAddress("dR", &dR, &b_dR);
}

void TrackJet::branchTrackJetTree(TTree *t)
{
    t->Branch("trackIdx", &trackIdx_out);
    t->Branch("jetIdx", &jetIdx_out);
    t->Branch("z", &z_out);
    t->Branch("ksi", &ksi_out);
    t->Branch("zt", &zt_out);
    t->Branch("angle", &angle_out);
    t->Branch("deta", &deta_out);
    t->Branch("dphi", &dphi_out);
    t->Branch("dR", &dR_out);
}

void TrackJet::clearTrackJetPairs()
{
    trackIdx_out.clear();
    jetIdx_out.clear();
    z_out.clear();
    ksi_out.clear();
    zt_out.clear();
    angle_out.clear();
    deta_out.clear();
    dphi_out.clear();
    dR_out.clear();
}

void TrackJet::makeTrackJetPair(Tracks &tTracks, Jets &tJets, int jetIndex)
{
    for (int i=0; i<tTracks.nTrk; ++i) {
        makeTrackJetPair(tTracks, i, tJets, jetIndex);
    }
}

void TrackJet::makeTrackJetPair(Tracks &tTracks, int trackIndex, Jets &tJets, int jetIndex)
{
        float tmp_dR = getDR(tTracks.trkEta[trackIndex], tTracks.trkPhi[trackIndex], tJets.jteta[jetIndex], tJets.jtphi[jetIndex]);
        if (tmp_dR >= coneRange)    return;

        float tmp_deta = getDETA(tTracks.trkEta[trackIndex], tJets.jteta[jetIndex]);
        float tmp_dphi = getDPHI(tTracks.trkPhi[trackIndex], tJets.jtphi[jetIndex]);

        TVector3 v1, v2;
        v1.SetPtEtaPhi(tTracks.trkPt[trackIndex], tTracks.trkEta[trackIndex], tTracks.trkPhi[trackIndex]);
        v2.SetPtEtaPhi(tJets.jtpt[jetIndex], tJets.jteta[jetIndex], tJets.jtphi[jetIndex]);

        // z = (momentum of track projected to jet axis) / jet momentum = (track momentum * cos(angle(track,jet))) / jet momentum
        float tmp_angle = v1.Angle(v2);
        float tmp_z = v1.Mag()*TMath::Cos(tmp_angle)/v2.Mag();
        float tmp_ksi = TMath::Log(1/tmp_z);
        float tmp_zt = tTracks.trkPt[trackIndex] / tJets.jtpt[jetIndex];

        trackIdx_out.push_back(trackIndex);
        jetIdx_out.push_back(jetIndex);
        z_out.push_back(tmp_z);
        ksi_out.push_back(tmp_ksi);
        zt_out.push_back(tmp_zt);
        angle_out.push_back(tmp_angle);
        deta_out.push_back(tmp_deta);
        dphi_out.push_back(tmp_dphi);
        dR_out.push_back(tmp_dR);
}

void TrackJet::makeTrackJetPairs(Tracks &tTracks, Jets &tJets)
{
    for (int i=0; i<tJets.nref; ++i) {
        makeTrackJetPair(tTracks, tJets, i);
    }
}

#endif /* TREEHEADERS_TRACKJETTREE_H_ */
