#include "gtest/gtest.h"

#include "tables-manager/DBTablesManager.h"

TEST(DBTablesManager, example) {
    auto dbContext = std::make_shared<DatabaseContext>("./SQLITE_TEMP.db");
    auto tablesManager = std::make_shared<DBTablesManager>(dbContext);

    tablesManager->insert("A", {0, "lean"});
    tablesManager->insert("A", {1, "sweater"});
    tablesManager->insert("A", {2, "frank"});
    tablesManager->insert("A", {3, "violation"});
    tablesManager->insert("A", {4, "quality"});
    tablesManager->insert("A", {5, "precision"});

    tablesManager->insert("B", {3, "proposal"});
    tablesManager->insert("B", {4, "example"});
    tablesManager->insert("B", {5, "lake"});
    tablesManager->insert("B", {6, "flour"});
    tablesManager->insert("B", {7, "wonder"});
    tablesManager->insert("B", {8, "selection"});

    auto intersectResult = tablesManager->intersect();
    ASSERT_EQ(intersectResult.size(), 3);
    ASSERT_EQ(intersectResult[0].id, 3);
    ASSERT_EQ(intersectResult[0].name1, "violation");
    ASSERT_EQ(intersectResult[0].name2, "proposal");
    ASSERT_EQ(intersectResult[1].id, 4);
    ASSERT_EQ(intersectResult[1].name1, "quality");
    ASSERT_EQ(intersectResult[1].name2, "example");
    ASSERT_EQ(intersectResult[2].id, 5);
    ASSERT_EQ(intersectResult[2].name1, "precision");
    ASSERT_EQ(intersectResult[2].name2, "lake");

    auto symmetricDifferenceResult = tablesManager->symmetricDifference();
    ASSERT_EQ(symmetricDifferenceResult.size(), 6);
    ASSERT_EQ(symmetricDifferenceResult[0].id, 0);
    ASSERT_EQ(symmetricDifferenceResult[0].name1, "lean");
    ASSERT_EQ(symmetricDifferenceResult[0].name2, "");
    ASSERT_EQ(symmetricDifferenceResult[1].id, 1);
    ASSERT_EQ(symmetricDifferenceResult[1].name1, "sweater");
    ASSERT_EQ(symmetricDifferenceResult[1].name2, "");
    ASSERT_EQ(symmetricDifferenceResult[2].id, 2);
    ASSERT_EQ(symmetricDifferenceResult[2].name1, "frank");
    ASSERT_EQ(symmetricDifferenceResult[2].name2, "");
    ASSERT_EQ(symmetricDifferenceResult[3].id, 6);
    ASSERT_EQ(symmetricDifferenceResult[3].name1, "");
    ASSERT_EQ(symmetricDifferenceResult[3].name2, "flour");
    ASSERT_EQ(symmetricDifferenceResult[4].id, 7);
    ASSERT_EQ(symmetricDifferenceResult[4].name1, "");
    ASSERT_EQ(symmetricDifferenceResult[4].name2, "wonder");
    ASSERT_EQ(symmetricDifferenceResult[5].id, 8);
    ASSERT_EQ(symmetricDifferenceResult[5].name1, "");
    ASSERT_EQ(symmetricDifferenceResult[5].name2, "selection");

    tablesManager->truncate("A");
    tablesManager->truncate("B");

    ASSERT_EQ(tablesManager->intersect().size(), 0);
    ASSERT_EQ(tablesManager->symmetricDifference().size(), 0);

    std::remove("./SQLITE_TEMP.db");
}