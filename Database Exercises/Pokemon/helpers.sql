-- COMP3311 25T1 Ass2 ... SQL helper Views/Functions
-- Add any views or functions you need into this file
-- Note: it must load without error into a freshly created Pokemon database

-- The `dbpop()` function is provided for you in the dump file
-- This is provided in case you accidentally delete it

DROP TYPE IF EXISTS Population_Record CASCADE;
CREATE TYPE Population_Record AS (
	Tablename Text,
	Ntuples   Integer
);

DROP TYPE IF EXISTS Mon_Move_Req_Record;
CREATE TYPE Mon_Move_Req_Record AS (
	Move        Text,
    Num_Games   Integer,
    Lvl_Average Integer
);

CREATE OR REPLACE FUNCTION DBpop()
    RETURNS SETOF Population_Record
    AS $$
        DECLARE
            rec Record;
            qry Text;
            res Population_Record;
            num Integer;
        BEGIN
            FOR rec IN SELECT tablename FROM pg_tables WHERE schemaname='public' ORDER BY tablename LOOP
                qry := 'SELECT count(*) FROM ' || quote_ident(rec.tablename);

                EXECUTE qry INTO num;

                res.tablename := rec.tablename;
                res.ntuples   := num;

                RETURN NEXT res;
            END LOOP;
        END;
    $$ LANGUAGE plpgsql
;

--
-- Example Views/Functions
-- These Views/Functions may or may not be useful to you.
-- You may modify or delete them as you see fit.
--

-- `Move_Learning_Info`
-- The `Learnable_Moves` table is a relation between Pokemon, Moves, Games and Requirements.
-- As it just consists of foreign keys, it is not very easy to read.
-- This view makes it easier to read by displaying the names of the Pokemon, Moves and Games instead of their IDs.
CREATE OR REPLACE VIEW Move_Learning_Info(Pokemon, Move, Game, Requirement) AS
    SELECT
        P.Name,
        M.Name,
        G.Name,
        R.Assertion
    FROM
        Learnable_Moves AS L
        JOIN Pokemon AS P
        ON Learnt_By = P.ID
        JOIN Games AS G
        ON Learnt_In = G.ID
        JOIN Moves AS M
        ON Learns = M.ID
        JOIN Requirements AS R
        ON Learnt_When = R.ID
;

-- `Super_Effective`
-- This function takes a type name and
-- returns a set of all types that it is super effective against (multiplier > 100)
-- eg Water is super effective against Fire, so `Super_Effective('Water')` will return `Fire` (amongst others)
CREATE OR REPLACE FUNCTION Super_Effective(_Type Text)
    RETURNS SETOF Text
    AS $$
        SELECT
            B.Name
        FROM
            Types AS A
            JOIN Type_Effectiveness AS E
            ON A.ID = E.Attacking
            JOIN Types AS B
            ON B.ID = E.Defending
        WHERE
            A.Name = _Type
            AND
            E.Multiplier > 100
    $$ LANGUAGE SQL
;

--
-- Your Views/Functions Below Here
-- Remember This file must load into a clean Pokemon database in one pass without any error
-- NOTICEs are fine, but ERRORs are not
-- Views/Functions must be defined in the correct order (dependencies first)
-- eg if my_supper_clever_function() depends on my_other_function() then my_other_function() must be defined first
-- Your Views/Functions Below Here
--
CREATE OR REPLACE VIEW Game_Data(Game_Name, National_ID, Egg_Group) AS
    SELECT
        G.Name,
        P_Dex.National_ID,
        Groups.Egg_Group
    FROM
        Games AS G
        JOIN Pokedex AS P_Dex
        ON G.ID = P_Dex.Game
        LEFT JOIN In_Group AS Groups
        ON Groups.pokemon = P_Dex.National_ID
;

CREATE OR REPLACE VIEW Mon_Egg_Count(Game_Name, Num_Egg_Group, Num_Pokemon) AS
    SELECT
        G.Game_Name,
        COUNT(DISTINCT (G.Game_Name, G.Egg_Group)) FILTER (
            WHERE G.Egg_Group IS NOT NULL
        ) AS Num_Egg_Group,
        COUNT(DISTINCT (G.Game_Name, G.National_ID)) AS Num_Pokemon
    FROM Game_Data AS G
    GROUP BY Game_Name
;

CREATE OR REPLACE VIEW Learnable_Moves_Type_Data(Type_Name, Move_ID, Mon_ID) AS
    SELECT
        T.Name AS Type_Name,
        LM.Learns AS Move_ID,
        P.ID AS Mon_ID
    FROM
        Types AS T
        JOIN Pokemon AS P
        ON P.First_Type = T.ID
        JOIN Learnable_Moves AS LM
        ON LM.Learnt_By = P.ID
        JOIN Moves AS M
        ON LM.Learns = M.ID
    WHERE T.ID = M.Of_Type
    GROUP BY
        Type_Name,
        Mon_ID,
        Move_ID
;

CREATE OR REPLACE VIEW All_Moves_Type_Data(Type_Name, Move_ID) AS
    SELECT
        T.Name AS Type_Name,
        M.ID AS Move_ID
    FROM
        Types AS T
        JOIN Moves AS M
        ON M.Of_Type = T.ID
    GROUP BY
        Type_Name,
        Move_ID
;

CREATE OR REPLACE VIEW Move_Type_Count(Type_Name, Num_Moves) AS
    SELECT
        MT.Type_Name AS Type_Name,
        COUNT(DISTINCT MT.Move_ID) AS Num_Moves
    FROM All_Moves_Type_Data AS MT
    GROUP BY Type_Name
;

CREATE OR REPLACE VIEW Mon_Move_Type_Count(Type_Name, Pokemon, Num_Mons_Moves) AS
    SELECT
        LMT.Type_Name AS Type_Name,
        LMT.Mon_ID AS Pokemon,
        COUNT(DISTINCT (LMT.Move_ID, LMT.Mon_ID)) AS Num_Mons_Moves
    FROM Learnable_Moves_Type_Data AS LMT
    GROUP BY
        Type_Name,
        Pokemon
;

CREATE OR REPLACE VIEW Type_10_Moves_Count(Type_Name, Num_Mons) AS
    SELECT
        PMTC.Type_Name AS Type_Name,
        COUNT(DISTINCT PMTC.Pokemon) FILTER (
            WHERE PMTC.Num_Mons_Moves > 10
        ) AS Num_Mons
    FROM Mon_Move_Type_Count AS PMTC
    GROUP BY Type_Name
;

CREATE OR REPLACE VIEW Type_Moves_Final_Count(Type_Name, Num_Moves, Num_Mons) AS
    SELECT
        M_List.Type_Name AS Type_Name,
        M_List.Num_Moves AS Num_Moves,
        P_10_List.Num_Mons AS Num_Mons
    FROM
        Move_Type_Count AS M_List
        JOIN Type_10_Moves_Count AS P_10_List
        ON M_List.Type_Name = P_10_List.Type_Name
;

CREATE OR REPLACE VIEW Move_Info_Level_Reqs(Pokemon, Move, Game, Requirement, Level_Req) AS
    SELECT
        L_Info.Pokemon AS Pokemon,
        L_Info.Move AS Move,
        L_Info.Game AS Game,
        L_Info.Requirement AS Requirement,
        SUBSTRING(L_Info.Requirement FROM '^Level: (\d+)$')::INT AS Level_Req
    FROM Move_Learning_Info AS L_Info
    WHERE L_Info.Requirement LIKE 'Level: %'
    GROUP BY
        Pokemon,
        Move,
        Game,
        Requirement,
        Level_Req
;

CREATE OR REPLACE VIEW Mon_Moves_Game_Count(Pokemon, Move, Num_Games, Lvl_Average) AS
    SELECT
        L_Info.Pokemon AS Pokemon,
        L_Info.Move AS Move,
        COUNT(DISTINCT(L_Info.Pokemon, L_Info.Move, L_Info.Game)) AS Num_Games,
        ROUND(AVG(L_Info.Level_Req)) AS Lvl_Average
    FROM Move_Info_Level_Reqs AS L_Info
    GROUP BY
        Pokemon,
        Move
    HAVING COUNT(DISTINCT(L_Info.Pokemon, L_Info.Move, L_Info.Game)) >= 30
;

CREATE OR REPLACE FUNCTION Check_Mon_Exists(Poke_Search Text)
    RETURNS SETOF Text
    AS $$
        SELECT P.name
        FROM Pokemon AS P
        WHERE P.name = Poke_Search
    $$ LANGUAGE SQL
;

CREATE OR REPLACE FUNCTION Get_Mon_Mov_Req_Data(Poke_Search Text)
    RETURNS SETOF Mon_Move_Req_Record
    AS $$
        SELECT
            List.Move,
            List.Num_Games,
            List.Lvl_Average
        FROM Mon_Moves_Game_Count AS List
        WHERE List.Pokemon = Poke_Search
    $$ LANGUAGE SQL
;

CREATE OR REPLACE VIEW Evo_Tree AS
    WITH RECURSIVE tree AS (
        SELECT
            E.ID,
            -1 AS Prev_Evo_ID,
            E.Pre_Evolution,
            E.Post_Evolution,
            1 AS depth,
            ARRAY[-1, E.ID] AS ID_Path,
            ARRAY[P_Pre.Name, P_Post.Name] AS Name_Path,
            E.Pre_Evolution AS Base_Mon_ID,
            P_Pre.Name AS Base_Mon_Name
        FROM
            Evolutions AS E
            JOIN Pokemon AS P_Pre
            ON P_Pre.ID = E.Pre_Evolution
            JOIN Pokemon AS P_Post
            ON P_Post.ID = E.Post_Evolution
        WHERE E.Pre_Evolution NOT IN (SELECT Post_Evolution FROM Evolutions)

        UNION ALL

        SELECT
            E.ID,
            T.ID AS Prev_Evo_ID,
            E.Pre_Evolution,
            E.Post_Evolution,
            T.Depth + 1,
            T.ID_Path || E.ID,
            T.Name_Path || P.Name,
            T.Base_Mon_ID,
            T.Base_Mon_Name
        FROM
            Evolutions AS E
            JOIN tree AS T
            ON E.Pre_Evolution = T.Post_Evolution
            JOIN Pokemon AS P
            ON P.ID = E.Post_Evolution
    )
    SELECT * FROM tree
;

CREATE OR REPLACE VIEW Final_Evo_Paths AS
    WITH Final_Evos AS (
        SELECT DISTINCT E.Post_Evolution AS Mon_ID
        FROM Evo_Tree AS E
        WHERE E.Post_Evolution NOT IN (SELECT Pre_Evolution FROM Evolutions)
    )
    SELECT
        E.Base_Mon_ID AS Base_Mon_ID,
        E.Base_Mon_Name AS Base_Mon_Name,
        E.ID_Path,
        E.Name_Path
    FROM
        Evo_Tree AS E
        JOIN Final_Evos AS F
        ON E.Post_Evolution = F.Mon_ID
    ORDER BY
        E.Base_Mon_ID,
        E.Name_Path
;


CREATE OR REPLACE VIEW Family_Groups AS
    WITH flattened AS (
        SELECT
            Base_Mon_Name,
            unnest(Name_Path) AS Name
        FROM Final_Evo_Paths
    ),
    families AS (
        SELECT
            Base_Mon_Name,
            ARRAY_AGG(DISTINCT Name) AS Family
        FROM flattened
        GROUP BY Base_Mon_Name
    )
    SELECT
        E.Base_Mon_ID,
        E.Base_Mon_Name,
        E.ID_Path,
        E.Name_Path,
        F.Family
    FROM 
        Final_Evo_Paths AS E
        JOIN families AS F 
        ON E.Base_Mon_Name = F.Base_Mon_Name
;

CREATE OR REPLACE VIEW All_Mons_Evo_Tree(
    Mon_ID, 
    Mon_Name, 
    Evo_Depth, 
    Evo_ID_Path, 
    Family, 
    Base_Mon_Name, 
    Evo_ID, 
    Prev_Evo_ID, 
    Evo_Req_ID, 
    Evo_Req_Text, 
    Evo_Req_Inv
) AS
    SELECT
        P.ID AS Mon_ID,
        P.Name AS Mon_Name,
        COALESCE(E_Post.Depth, 0) AS Evo_Depth,
        COALESCE(F_Post.ID_Path, F_Base.ID_Path, ARRAY[-1]) AS Evo_ID_Path,
        COALESCE(F_Post.Family, F_Base.Family, ARRAY[P.Name]) AS Family,
        COALESCE(E_Post.Base_Mon_Name, P.Name) AS Base_Mon_Name,
        COALESCE(E_Post.ID, -1) AS Evo_ID,
        COALESCE(E_Post.Prev_Evo_ID, -2) AS Prev_Evo_ID,
        E_Req.Requirement AS Evo_Req_ID,
        R.Assertion AS Evo_Req_Text,
        E_Req.Inverted AS Evo_Req_Inv
    FROM
        Pokemon AS P
        LEFT JOIN Evo_Tree AS E_Post
        ON P.ID = E_Post.Post_Evolution
        LEFT JOIN Evo_Tree AS E_Base
        ON P.ID = E_Base.Base_Mon_ID
        LEFT JOIN Family_Groups AS F_Post
        ON E_Post.Base_Mon_ID = F_Post.Base_Mon_ID
        LEFT JOIN Family_Groups AS F_Base
        ON E_Base.Base_Mon_ID = F_Base.Base_Mon_ID
        LEFT JOIN Evolution_Requirements AS E_Req
        ON E_Req.Evolution = E_Post.ID
        LEFT JOIN Requirements AS R
        ON E_Req.Requirement = R.ID
    GROUP BY
        Mon_ID,
        Mon_Name,
        E_Post.Base_Mon_Name,
        Evo_Depth,
        Evo_ID_Path,
        F_Post.Family,
        F_Base.Family,
        Evo_ID,
        E_Post.Prev_Evo_ID,
        Evo_Req_ID,
        Evo_Req_Text,
        Evo_Req_Inv
    ORDER BY
        Mon_Name,
        Evo_ID_Path,
        Evo_ID,
        Evo_Req_ID
;

CREATE OR REPLACE VIEW All_Mons_Evo_Data(
    Mon_ID, 
    Src_Name, 
    Evo_Name, 
    Evo_ID,
    Prev_Evo_ID,
    Evo_ID_Path,
    Evo_Depth,
    Evo_Req_ID, 
    Evo_Req_Text, 
    Evo_Req_Inv, 
    Base_Name
) AS
    SELECT DISTINCT ON (Src_Name, Evo_Depth, Evo_Name, Evo_ID, Prev_Evo_ID, Evo_Req_ID)
        T.Mon_ID,
        T.Mon_Name AS Src_Name,
        S.Group_Name AS Evo_Name,
        M.Evo_ID,
        M.Prev_Evo_ID,
        M.Evo_ID_Path,
        M.Evo_Depth,
        M.Evo_Req_ID,
        M.Evo_Req_Text,
        M.Evo_Req_Inv,
        M.Base_Mon_Name AS Base_Name
    FROM
        All_Mons_Evo_Tree AS T
        CROSS JOIN unnest(T.Family) AS S(Group_Name)
        LEFT JOIN All_Mons_Evo_Tree AS M
        ON M.Mon_Name = S.Group_Name
    WHERE M.Evo_ID = ANY(M.Evo_ID_Path)
    ORDER BY
        Src_Name,
        Evo_Depth,
        Evo_Name,
        Evo_ID,
        Prev_Evo_ID,
        Evo_Req_ID
;



CREATE OR REPLACE FUNCTION Check_Partial_Mon_Exists(Poke_Search Text)
    RETURNS SETOF Text
    AS $$
        SELECT P.Name
        FROM Pokemon AS P
        WHERE P.Name ILIKE '%' || Poke_Search || '%'
    $$ LANGUAGE SQL;
;

DROP TYPE IF EXISTS Mon_Evo_Record;
CREATE TYPE Mon_Evo_Record AS (
	Src_Name        Text,
    Evo_Name        Text,
    Evo_Depth       Integer,
    Evo_ID          Integer,
    Evo_Req_Text    Text,
    Evo_Req_Inv     Boolean
);

DROP TYPE IF EXISTS Evo_Key;
CREATE TYPE Evo_Key AS (
	Prev_Evo_ID Integer,
    Evo_ID Integer,
    Evo_Req_ID Integer
);

CREATE OR REPLACE FUNCTION Get_Mon_Evo_Data(Poke_Search Text)
    RETURNS SETOF Mon_Evo_Record
    AS $$
        DECLARE
            all_evo_data All_Mons_Evo_Data[];
            evo_data RECORD;
            
            i Integer := 0;
            start_src_i Integer := 1; 
            redo Boolean := FALSE;

            prev_src_mon Text;

            curr_path_tracker Integer[] := ARRAY[-2]::INTEGER[];

            curr_base_mon Text;

            curr_evo_key Evo_Key;
            prev_evo_keys Evo_Key[];
        BEGIN
            SELECT
                ARRAY_AGG(ROW(
                    Mon_ID, 
                    Src_Name, 
                    Evo_Name, 
                    Evo_ID,
                    Prev_Evo_ID,
                    Evo_ID_Path,
                    Evo_Depth,
                    Evo_Req_ID, 
                    Evo_Req_Text, 
                    Evo_Req_Inv, 
                    Base_Name
                )::All_Mons_Evo_Data)
            INTO all_evo_data
            FROM All_Mons_Evo_Data
            WHERE Src_Name ILIKE '%' || Poke_Search || '%';
            
            WHILE i <= array_length(all_evo_data, 1) LOOP
                i := i + 1;
                IF i > array_length(all_evo_data, 1) THEN
                    IF Redo THEN
                        Redo := FALSE;
                      
                        i = start_src_i;
                        curr_path_tracker := curr_path_tracker[1:array_upper(curr_path_tracker, 1) - 1];
                    ELSE
                        EXIT;
                    END IF;
                END IF;

                evo_data := all_evo_data[i];

                curr_evo_key := ROW(
                    evo_data.Prev_Evo_ID,
                    evo_data.Evo_ID,
                    evo_data.Evo_Req_ID
                )::Evo_Key;

                IF NOT (evo_data.Src_Name = prev_src_mon) THEN
                    IF Redo THEN
                        Redo := FALSE;
                        i = start_src_i;
                        curr_path_tracker := curr_path_tracker[1:array_upper(curr_path_tracker, 1) - 1];
                    ELSE
                        -- reset trackers
                        prev_evo_keys := ARRAY[]::Evo_Key[];
                        curr_path_tracker := ARRAY[-2]::INTEGER[];
                        curr_base_mon := evo_data.base_name;
                        start_src_i := i;
                    END IF;
                END IF;

                evo_data := all_evo_data[i];

                curr_evo_key := ROW(
                    evo_data.Prev_Evo_ID,
                    evo_data.Evo_ID,
                    evo_data.Evo_Req_ID
                )::Evo_Key;

                IF curr_evo_key = ANY(prev_evo_keys) THEN
                    CONTINUE;
                END IF;

                IF curr_path_tracker[array_upper(curr_path_tracker, 1)] = evo_data.Evo_ID THEN
                    prev_src_mon := evo_data.Src_Name;
                    prev_evo_keys = prev_evo_keys || curr_evo_key;
                    redo := TRUE;
                    
                    RETURN NEXT (
                        evo_data.Src_Name,
                        evo_data.Evo_Name,
                        evo_data.Evo_Depth,
                        evo_data.Evo_ID,
                        evo_data.Evo_Req_Text,
                        evo_data.Evo_Req_Inv
                    );
                    
                    CONTINUE;
                END IF;

                IF curr_path_tracker[array_upper(curr_path_tracker, 1)] = evo_data.Prev_Evo_ID THEN
                    IF NOT(evo_data.Evo_Depth = 2) THEN
                        curr_path_tracker := curr_path_tracker || evo_data.Evo_ID;
                    END IF;
                    prev_src_mon := evo_data.Src_Name;
                    prev_evo_keys = prev_evo_keys || curr_evo_key;
                    redo := TRUE;
                    RETURN NEXT (
                        evo_data.Src_Name,
                        evo_data.Evo_Name,
                        evo_data.Evo_Depth,
                        evo_data.Evo_ID,
                        evo_data.Evo_Req_Text,
                        evo_data.Evo_Req_Inv
                    );
                    
                    CONTINUE;
                END IF;
            END LOOP;        
        END;
    $$ LANGUAGE plpgsql;
;

CREATE OR REPLACE VIEW All_Mon_Types(Name, First_Type, Second_Type) AS
    SELECT
        P.Name,
        First_T.Name AS First_Type,
        Second_T.NAme AS Second_Type
    FROM
        Pokemon AS P
        JOIN Types AS First_T
        ON First_T.ID = P.First_Type
        LEFT JOIN Types AS Second_T
        ON Second_T.ID = P.Second_Type
;

CREATE OR REPLACE VIEW Evo_Types_Paths(Base_Mon, Name_Path, Evo_Mon, Types) AS
    WITH flattened AS (
        SELECT
            F.Base_Mon_Name AS Base_Mon,
            F.Name_Path,
            unnest(F.Name_Path) AS Evo_Mon
        FROM
            Final_Evo_Paths AS F
    )
    SELECT
        F.Base_Mon,
        F.Name_Path,
        F.Evo_Mon,
        CASE
            WHEN PT.Second_Type IS NULL
            THEN ARRAY[PT.First_Type]
            ELSE ARRAY[PT.First_Type, PT.Second_Type]
        END AS Types
    FROM
        flattened AS F
        LEFT JOIN All_Mon_Types AS PT
        ON PT.Name = F.Evo_Mon
    GROUP BY
        Base_Mon,
        Name_Path,
        Evo_Mon,
        Types
    ORDER BY
        Base_Mon
;

CREATE OR REPLACE VIEW Grouped_Evo_Types_Paths AS
    WITH evo_types AS (
        SELECT
            Base_Mon,
            Evo_Mon,
            Name_Path,
            unnest(Types) AS Type
        FROM Evo_Types_Paths
    ),
    grouped_types AS (
        SELECT
            Base_Mon,
            Name_Path,
            COUNT(DISTINCT Evo_Mon) AS Stage_Count,
            COUNT(*) FILTER (WHERE Type IS NOT NULL) AS Type_Inst,
            ARRAY_AGG(DISTINCT Type) AS Existing_Types,
            ARRAY(
                SELECT Type
                FROM (
                    SELECT 
                        Type, 
                        COUNT(*) AS n
                    FROM evo_types AS ET
                    WHERE ET.Name_Path = Evo.Name_Path
                    GROUP BY Type
                ) AS Sub
                WHERE 
                    n = (
                        SELECT COUNT(DISTINCT Evo_Mon)
                        FROM evo_types
                        WHERE Name_Path = Evo.Name_Path
                    )
            ) AS Common_Types
        FROM evo_types AS Evo
        GROUP BY 
            Base_Mon,
            Name_Path
    )
    SELECT
        Base_Mon,
        Name_Path,
        Common_Types,
        Existing_Types
    FROM grouped_types
;

CREATE OR REPLACE VIEW Evo_Req_Paths(Base_Mon, Name_Path, ID_Path, Evo_Req) AS
    SELECT
        F.Base_Mon_Name AS Base_Mon,
        F.Name_Path,
        F.ID_Path,
        CASE
            WHEN E.Inverted 
            THEN 'Not ' || R.Assertion 
            ELSE R.Assertion
        END AS Evo_Req
    FROM
        Final_Evo_Paths AS F
        JOIN Evolution_Requirements AS E
        ON E.Evolution = ANY(F.ID_Path)
        JOIN Requirements AS R
        ON E.Requirement = R.ID
    GROUP BY
        Base_Mon,
        Name_Path,
        ID_Path,
        Evo_Req
;

CREATE OR REPLACE VIEW Grouped_Evo_Reqs_Paths(Base_Mon, Name_Path, ID_Path, Evo_Reqs) AS
    SELECT
        E.Base_Mon,
        E.Name_Path,
        E.ID_Path,
        ARRAY_AGG(DISTINCT E.Evo_Req) AS Evo_Reqs
    FROM Evo_Req_Paths AS E
    GROUP BY
        E.Base_Mon,
        E.Name_Path,
        E.ID_Path
;

CREATE OR REPLACE VIEW Final_Evo_Req_Types_Paths(Base_Mon, Final_Evo, Depth, ID_Path, Evo_Reqs, Existing_Types, Common_Types) AS
    SELECT
        F.Base_Mon_Name AS Base_Mon,
        F.Name_Path[array_length(F.Name_Path,1)] AS Final_Evo, 
        array_length(F.Name_Path,1) - 1 AS Depth,
        F.ID_Path,
        R.Evo_Reqs,
        T.Existing_Types,
        T.Common_Types
    FROM
        Final_Evo_Paths AS F
        JOIN Grouped_Evo_Reqs_Paths AS R
        ON R.ID_Path = F.ID_Path
        JOIN Grouped_Evo_Types_Paths AS T
        ON T.Name_Path = F.Name_Path
    GROUP BY
        F.Base_Mon_Name,
        Final_Evo,
        Depth,
        F.ID_Path,
        Evo_Reqs,
        Existing_Types,
        Common_Types
    ORDER BY
        Depth DESC,
        Final_Evo,
        Base_Mon
        
        
;

DROP TYPE IF EXISTS Short_Evo_Branch;
CREATE TYPE Short_Evo_Branch AS (
	Base_Mon    Text,
    Final_Evo   Text,
    Depth       Integer
);

CREATE OR REPLACE FUNCTION Get_Valid_Evos(Requirements Text, Types Text)
    RETURNS SETOF Short_Evo_Branch
    AS $$
        DECLARE
            evo_rec RECORD;
            
            curr_branch Short_Evo_Branch;
            listed Short_Evo_Branch[];

            input_reqs Text[];
            input_types Text[];
            
            type_text Text;
            type_common Text[];
            type_remove Text[];

            para_req_text Text;
            data_req_text Text;
            req_w_num Text[] := ARRAY[]::Text[];
            req_w_num_text Text[] := ARRAY[]::Text[];
            req_pure_text Text[] := ARRAY[]::Text[]; 
            
            i Integer;
            found Boolean;
            
        BEGIN
            input_reqs := string_to_array(Requirements, ';');
            input_types := string_to_array(Types, ';');

            FOREACH type_text IN ARRAY input_types LOOP
                IF type_text LIKE '^%' THEN
                    type_remove := type_remove || SUBSTRING(type_text FROM 2);
                ELSE
                    type_common := type_common || type_text;
                END IF;
            END LOOP;

            FOREACH para_req_text IN ARRAY input_reqs LOOP
                IF para_req_text ~ '\d' THEN
                    req_w_num := req_w_num || para_req_text;
                    req_w_num_text := req_w_num_text || REGEXP_REPLACE(para_req_text, '\d', '', 'g');
                ELSE
                    req_pure_text := req_pure_text || para_req_text;
                END IF;
            END LOOP;

            FOR evo_rec IN
                SELECT *
                FROM Final_Evo_Req_Types_Paths
            LOOP
                curr_branch = (
                    evo_rec.Base_Mon,
                    evo_rec.Final_Evo,
                    evo_rec.depth
                );

                IF curr_branch = ANY(listed) THEN
                    CONTINUE;
                END IF;

                IF array_length(type_remove, 1) > 0 THEN
                    found := FALSE;
                    FOREACH type_text IN ARRAY evo_rec.Existing_Types LOOP
                        IF type_text ILIKE ANY(type_remove) THEN
                            found := TRUE;
                        END IF;
                    END LOOP;

                    IF found THEN
                        CONTINUE;
                    END IF;
                END IF;
                
                IF array_length(type_common, 1) > 0 THEN
                    found := FALSE;
                    FOREACH type_text IN ARRAY evo_rec.Common_Types LOOP
                        IF type_text ILIKE ANY(type_common) THEN
                            found := TRUE;
                        END IF;
                    END LOOP;

                    IF NOT found THEN
                        CONTINUE;
                    END IF;
                END IF;

                found := FALSE;
                FOREACH data_req_text IN ARRAY evo_rec.Evo_Reqs LOOP
                    IF data_req_text ~ '\d' THEN
                        IF NOT (REGEXP_REPLACE(data_req_text, '\d', '', 'g') ILIKE ANY(req_w_num_text)) THEN
                            found := TRUE;
                        ELSE
                            FOREACH para_req_text IN ARRAY req_w_num LOOP
                                IF (REGEXP_REPLACE(data_req_text, '\d', '', 'g') ILIKE REGEXP_REPLACE(para_req_text, '\d', '', 'g')) 
                                AND SUBSTRING(data_req_text FROM '\d+')::Integer > SUBSTRING(para_req_text FROM '\d+')::Integer THEN
                                    found := TRUE;
                                END IF;
                            END LOOP;
                        END IF;
                    ELSE
                        IF NOT (data_req_text ILIKE ANY(req_pure_text)) THEN
                            found := TRUE;
                        END IF;
                    END IF;
                END LOOP;

                IF found THEN
                    CONTINUE;
                END IF;

                listed = listed || curr_branch;
                RETURN NEXT curr_branch;
            END LOOP;            
        END;
    $$ LANGUAGE plpgsql
; 
