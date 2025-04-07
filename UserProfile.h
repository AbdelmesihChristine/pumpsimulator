#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <QString>

/**
 * @brief Data structure for storing user-specific insulin parameters.
 *
 * This version uses mmol/L for BG. Adjust if you prefer mg/dL.
 */
struct UserProfile {
    QString name;
    double basalRate;         // U/hour
    double carbRatio;         // grams of carbs per 1U
    double correctionFactor;  // mmol/L per 1U
    double targetGlucose;     // mmol/L

    UserProfile()
        : basalRate(0), carbRatio(0), correctionFactor(0), targetGlucose(5) {}

    UserProfile(const QString& nm, double basal, double carb, double cf, double tgt)
        : name(nm), basalRate(basal), carbRatio(carb), correctionFactor(cf), targetGlucose(tgt) {}
};

#endif // USERPROFILE_H
