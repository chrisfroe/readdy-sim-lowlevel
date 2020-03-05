/**
 * @file main.cpp
 * @brief Minimal example of a low-level readdy simulation using the single cpu kernel directly
 * @author chrisfroe
 * @date 05.03.20
 * @copyright BSD-3
 */

#include <readdy/kernel/singlecpu/SCPUKernel.h>

/**
 * Order of setup:
 * 1. Context
 * 2. Fill stateModel
 * 3. Actions
 * 4. The simulation loop
 */
int main(int argc, char **argv) {
    readdy::kernel::scpu::SCPUKernel kernel;

    // 1. Context
    auto &ctx = kernel.context();
    auto &types = ctx.particleTypes();
    types.add("A", 1.0);
    types.add("B", 1.0);
    ctx.potentials().addHarmonicRepulsion("A", "B", 1., 1.);
    auto &topReg = ctx.topologyRegistry();
    topReg.addType("T");
    readdy::api::Bond bond{1., 1., readdy::api::BondType::HARMONIC};
    topReg.configureBondPotential("A", "A", bond);
    topReg.configureBondPotential("B", "A", bond);

    // 2. Fill stateModel
    std::vector<readdy::model::TopologyParticle> particles{
            {0., 0., -1., types.idOf("A")},
            {0., 0., 1., types.idOf("A")},
            {0., 0., 3., types.idOf("B")}
    };
    auto graphTop = kernel.stateModel().addTopology(topReg.idOf("T"), particles);
    auto &graph = graphTop->graph();
    graph.addEdgeBetweenParticles(0, 1);
    graph.addEdgeBetweenParticles(1, 2);

    // 3. Actions
    readdy::scalar timeStep = 1.0;

    auto integrator = kernel.actions().eulerBDIntegrator(timeStep);
    auto forces = kernel.actions().calculateForces();
    auto initNeighborList = kernel.actions().createNeighborList(kernel.context().calculateMaxCutoff());
    auto neighborList = kernel.actions().updateNeighborList();
    auto reactions = kernel.actions().uncontrolledApproximation(timeStep);
    auto topologyReactions = kernel.actions().evaluateTopologyReactions(timeStep);

    readdy::model::actions::top::BreakConfig breakConfig;
    breakConfig.addBreakablePair(types.idOf("A"), types.idOf("B"), 0.9, 1e10);
    auto breakingBonds = kernel.actions().breakBonds(timeStep, breakConfig);

    // 4. The simulation loop
    size_t nSteps = 100;

    initNeighborList->perform();
    neighborList->perform();
    forces->perform();
    kernel.evaluateObservables(0);
    for (size_t t = 1; t < nSteps + 1; t++) {
        integrator->perform();
        neighborList->perform();
        forces->perform();
        reactions->perform();
        topologyReactions->perform();
        breakingBonds->perform();
        neighborList->perform();
        forces->perform();
        kernel.evaluateObservables(t);
    }
    return 0;
}
