#include "milp.h"
#include <map>
#include <unordered_set>
#include "util.h"

void solve_by_integrate_model(const std::unordered_map<std::string, std::vector<Block>>& pattern_block_map,
	std::unordered_map<std::string, std::vector<Block>>& selected_pattern_block_map)
{

	std::vector<std::string> all_patterns;
	std::vector<const Block*> all_blocks;

	for (const auto& pattern_blocks : pattern_block_map) {
		const auto& pattern = pattern_blocks.first;
		const auto& blocks = pattern_blocks.second;
		all_patterns.push_back(pattern);
		for (const auto& block : blocks) {
			all_blocks.push_back(&block);
		}
	}

	size_t n_patterns = all_patterns.size();
	size_t n_blocks = all_blocks.size();

	std::unordered_map<std::string, std::vector<size_t>> pattern_block_id_map;
	std::map<size_t, std::vector<size_t>> seq_id_block_ids_map;
	std::unordered_map<const Block*, size_t> block_id_map;
	for (size_t i = 0; i < n_blocks; i++)
	{
		pattern_block_id_map[all_blocks[i]->get_substring()].push_back(i);
		seq_id_block_ids_map[all_blocks[i]->get_sequence_id()].push_back(i);
		block_id_map[all_blocks[i]] = i;
	}


	IloEnv env;
	try {


		IloBoolVarArray pattern_vars(env, n_patterns);
		IloBoolVarArray block_vars(env, n_blocks);


		IloModel model(env);

		IloExpr obj_expr(env);

		for (size_t i = 0; i < n_patterns; i++)
		{
			obj_expr -= 0.6 * all_patterns[i].size() * pattern_vars[i];
			pattern_vars[i].setName(("p_" + std::to_string(i)).c_str());
		}
		for (size_t i = 0; i < n_blocks; i++)
		{
			obj_expr += 0.8 * all_blocks[i]->get_block_size() * block_vars[i];
			block_vars[i].setName(("b_" + std::to_string(i)).c_str());
		}

		model.add(IloMaximize(env, obj_expr));


		for (size_t i = 0; i < n_patterns; i++)
		{
			auto itr = pattern_block_id_map.find(all_patterns[i]);
			if (itr != pattern_block_id_map.end()) {
				for (const auto j : itr->second) {
					IloExpr expr(env);
					expr -= pattern_vars[i];
					expr += block_vars[j];
					model.add(expr <= 0);
				}
			}
			
		}

		//for (size_t i = 0; i < n_blocks; i++) {
		//	auto p_block_1 = all_blocks[i];
		//	for (auto j : seq_id_block_ids_map.at(p_block_1->get_sequence_id())) {
		//		auto p_block_2 = all_blocks[j];
		//		if (*p_block_1 == *p_block_2) {
		//			continue;
		//		}
		//		if (p_block_1->is_overlap(*p_block_2)) {
		//			IloExpr expr(env);
		//			expr += block_vars[i];
		//			expr += block_vars[j];
		//			model.add(expr <= 1);
		//		}
		//	}
		//}

		for (const auto& seq_id_block_ids : seq_id_block_ids_map) {
			std::vector<const Block*> seq_blocks;
			for (const auto block_id : seq_id_block_ids.second) {
				seq_blocks.push_back(all_blocks[block_id]);
			}
			std::vector<std::unordered_set<int>> maximal_cliques;
			find_all_maximal_cliques(seq_blocks, maximal_cliques);
			for (const auto& clique : maximal_cliques) {
				IloExpr expr(env);
				for (const auto seq_block_id : clique) {
					size_t block_id = block_id_map[seq_blocks[seq_block_id]];
					expr += block_vars[block_id];
				}
				model.add(expr <= 1);
			}
		}


		IloCplex cplex(env);
		cplex.extract(model);
		cplex.exportModel("model.lp");
		cplex.solve();

		cplex.writeSolution("sol.txt");

		std::cout << "Solution status: " << cplex.getStatus() << std::endl;

		std::cout << "Obj " << cplex.getObjValue() << std::endl;

		IloNum eps = cplex.getParam(IloCplex::Param::MIP::Tolerances::Integrality);
		for (size_t i = 0; i < n_blocks; i++) {
			if (cplex.getValue(block_vars[i]) > eps) {
				selected_pattern_block_map[all_blocks[i]->get_substring()].push_back(*all_blocks[i]);
			}
		}

	}
	catch (IloException& ex) {
		std::cerr << "Error: " << ex << std::endl;
	}
	catch (...) {
		std::cerr << "Error" << std::endl;
	}
}
