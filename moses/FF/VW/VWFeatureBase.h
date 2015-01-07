#pragma once

#include <string>
#include "Classifier.h"
#include "moses/TypeDef.h"
#include "moses/Util.h"
#include "moses/FF/StatelessFeatureFunction.h"

namespace Moses
{

class VWFeatureBase : public StatelessFeatureFunction
{
  public:
    VWFeatureBase(const std::string &line, bool isSource = true)
      :StatelessFeatureFunction(0, line)
    {
      ReadParameters();

      if(m_usedBy.empty())
        m_usedBy.push_back("VW0");

      for(std::vector<std::string>::const_iterator it = m_usedBy.begin();
          it != m_usedBy.end(); it++) {
        s_features[*it].push_back(this);
        if(isSource)
          s_sourceFeatures[*it].push_back(this);
        else
          s_targetFeatures[*it].push_back(this);
      }
    }

    bool IsUseable(const FactorMask &mask) const {
      return true;
    }

    // Official hooks do nothing
    void EvaluateInIsolation(const Phrase &source
                  , const TargetPhrase &targetPhrase
                  , ScoreComponentCollection &scoreBreakdown
                  , ScoreComponentCollection &estimatedFutureScore) const {}
    void EvaluateWithSourceContext(const InputType &input
                  , const InputPath &inputPath
                  , const TargetPhrase &targetPhrase
                  , const StackVec *stackVec
                  , ScoreComponentCollection &scoreBreakdown
                  , ScoreComponentCollection *estimatedFutureScore = NULL) const {}

    void EvaluateTranslationOptionListWithSourceContext(const InputType &input
                  , const TranslationOptionList &translationOptionList) const {}

    void EvaluateWhenApplied(const Hypothesis& hypo,
                  ScoreComponentCollection* accumulator) const {}
    void EvaluateWhenApplied(const ChartHypothesis &hypo,
                       ScoreComponentCollection* accumulator) const {}

    void SetParameter(const std::string& key, const std::string& value)
    {
      if (key == "used-by") {
        ParseUsedBy(value);
      } else if (key == "source-factors") {
        ParseFactorDefinition(value, m_sourceFactors);
      } else if (key == "target-factors") {
        ParseFactorDefinition(value, m_targetFactors);
      } else {
        StatelessFeatureFunction::SetParameter(key, value);
      }
    }

    static const std::vector<VWFeatureBase*>& GetFeatures(std::string name = "VW0") {
      UTIL_THROW_IF2(s_features.count(name) == 0, "No features registered for parent classifier: " + name);
      return s_features[name];
    }

    static const std::vector<VWFeatureBase*>& GetSourceFeatures(std::string name = "VW0") {
      UTIL_THROW_IF2(s_sourceFeatures.count(name) == 0, "No source features registered for parent classifier: " + name);
      return s_sourceFeatures[name];
    }

    static const std::vector<VWFeatureBase*>& GetTargetFeatures(std::string name = "VW0") {
      UTIL_THROW_IF2(s_targetFeatures.count(name) == 0, "No target features registered for parent classifier: " + name);
      return s_targetFeatures[name];
    }

    virtual void operator()(const InputType &input
                            , const InputPath &inputPath
                            , const WordsRange &sourceRange
                            , Discriminative::Classifier *classifier) const = 0;
    
    virtual void operator()(const InputType &input
                            , const InputPath &inputPath
                            , const TargetPhrase &targetPhrase
                            , Discriminative::Classifier *classifier) const = 0;
    
  protected:
    std::vector<FactorType> m_sourceFactors, m_targetFactors;

  private:
    void ParseFactorDefinition(const std::string &list, /* out */ std::vector<FactorType> &out)
    {
      std::vector<std::string> split = Tokenize(list, ",");
      Scan<FactorType>(out, split);
    }

    void ParseUsedBy(const std::string &usedBy) {
      Tokenize(m_usedBy, usedBy, ",");
    }

    std::vector<std::string> m_usedBy;
    static std::map<std::string, std::vector<VWFeatureBase*> > s_features;
    static std::map<std::string, std::vector<VWFeatureBase*> > s_sourceFeatures;
    static std::map<std::string, std::vector<VWFeatureBase*> > s_targetFeatures;
};

}
