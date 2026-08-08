const struct Decoration gDecorations[] =
{
    [DECOR_NONE] =
    {
        .id = DECOR_NONE,
        .name = _("SMALL DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DESK,
        .price = 0,
        .description = DecorDesc_SMALL_DESK,
        .tiles = DecorGfx_SMALL_DESK,
    },

    [DECOR_SMALL_DESK] =
    {
        .id = DECOR_SMALL_DESK,
        .name = _("SMALL DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DESK,
        .price = 3000,
        .description = DecorDesc_SMALL_DESK,
        .tiles = DecorGfx_SMALL_DESK,
    },

    [DECOR_POKEMON_DESK] =
    {
        .id = DECOR_POKEMON_DESK,
        .name = _("POKéMON DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DESK,
        .price = 3000,
        .description = DecorDesc_POKEMON_DESK,
        .tiles = DecorGfx_POKEMON_DESK,
    },

    [DECOR_HEAVY_DESK] =
    {
        .id = DECOR_HEAVY_DESK,
        .name = _("HEAVY DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x2,
        .category = DECORCAT_DESK,
        .price = 6000,
        .description = DecorDesc_HEAVY_DESK,
        .tiles = DecorGfx_HEAVY_DESK,
    },

    [DECOR_RAGGED_DESK] =
    {
        .id = DECOR_RAGGED_DESK,
        .name = _("RAGGED DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x2,
        .category = DECORCAT_DESK,
        .price = 6000,
        .description = DecorDesc_RAGGED_DESK,
        .tiles = DecorGfx_RAGGED_DESK,
    },

    [DECOR_COMFORT_DESK] =
    {
        .id = DECOR_COMFORT_DESK,
        .name = _("COMFORT DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x2,
        .category = DECORCAT_DESK,
        .price = 6000,
        .description = DecorDesc_COMFORT_DESK,
        .tiles = DecorGfx_COMFORT_DESK,
    },

    [DECOR_PRETTY_DESK] =
    {
        .id = DECOR_PRETTY_DESK,
        .name = _("PRETTY DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_DESK,
        .price = 9000,
        .description = DecorDesc_PRETTY_DESK,
        .tiles = DecorGfx_PRETTY_DESK,
    },

    [DECOR_BRICK_DESK] =
    {
        .id = DECOR_BRICK_DESK,
        .name = _("BRICK DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_DESK,
        .price = 9000,
        .description = DecorDesc_BRICK_DESK,
        .tiles = DecorGfx_BRICK_DESK,
    },

    [DECOR_CAMP_DESK] =
    {
        .id = DECOR_CAMP_DESK,
        .name = _("CAMP DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_DESK,
        .price = 9000,
        .description = DecorDesc_CAMP_DESK,
        .tiles = DecorGfx_CAMP_DESK,
    },

    [DECOR_HARD_DESK] =
    {
        .id = DECOR_HARD_DESK,
        .name = _("HARD DESK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_DESK,
        .price = 9000,
        .description = DecorDesc_HARD_DESK,
        .tiles = DecorGfx_HARD_DESK,
    },

    [DECOR_SMALL_CHAIR] =
    {
        .id = DECOR_SMALL_CHAIR,
        .name = _("SMALL CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_SMALL_CHAIR,
        .tiles = DecorGfx_SMALL_CHAIR,
    },

    [DECOR_POKEMON_CHAIR] =
    {
        .id = DECOR_POKEMON_CHAIR,
        .name = _("POKéMON CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_POKEMON_CHAIR,
        .tiles = DecorGfx_POKEMON_CHAIR,
    },

    [DECOR_HEAVY_CHAIR] =
    {
        .id = DECOR_HEAVY_CHAIR,
        .name = _("HEAVY CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_HEAVY_CHAIR,
        .tiles = DecorGfx_HEAVY_CHAIR,
    },

    [DECOR_PRETTY_CHAIR] =
    {
        .id = DECOR_PRETTY_CHAIR,
        .name = _("PRETTY CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_PRETTY_CHAIR,
        .tiles = DecorGfx_PRETTY_CHAIR,
    },

    [DECOR_COMFORT_CHAIR] =
    {
        .id = DECOR_COMFORT_CHAIR,
        .name = _("COMFORT CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_COMFORT_CHAIR,
        .tiles = DecorGfx_COMFORT_CHAIR,
    },

    [DECOR_RAGGED_CHAIR] =
    {
        .id = DECOR_RAGGED_CHAIR,
        .name = _("RAGGED CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_RAGGED_CHAIR,
        .tiles = DecorGfx_RAGGED_CHAIR,
    },

    [DECOR_BRICK_CHAIR] =
    {
        .id = DECOR_BRICK_CHAIR,
        .name = _("BRICK CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_BRICK_CHAIR,
        .tiles = DecorGfx_BRICK_CHAIR,
    },

    [DECOR_CAMP_CHAIR] =
    {
        .id = DECOR_CAMP_CHAIR,
        .name = _("CAMP CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_CAMP_CHAIR,
        .tiles = DecorGfx_CAMP_CHAIR,
    },

    [DECOR_HARD_CHAIR] =
    {
        .id = DECOR_HARD_CHAIR,
        .name = _("HARD CHAIR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CHAIR,
        .price = 2000,
        .description = DecorDesc_HARD_CHAIR,
        .tiles = DecorGfx_HARD_CHAIR,
    },

    [DECOR_RED_PLANT] =
    {
        .id = DECOR_RED_PLANT,
        .name = _("RED PLANT"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 3000,
        .description = DecorDesc_RED_PLANT,
        .tiles = DecorGfx_RED_PLANT,
    },

    [DECOR_TROPICAL_PLANT] =
    {
        .id = DECOR_TROPICAL_PLANT,
        .name = _("TROPICAL PLANT"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 3000,
        .description = DecorDesc_TROPICAL_PLANT,
        .tiles = DecorGfx_TROPICAL_PLANT,
    },

    [DECOR_PRETTY_FLOWERS] =
    {
        .id = DECOR_PRETTY_FLOWERS,
        .name = _("PRETTY FLOWERS"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 3000,
        .description = DecorDesc_PRETTY_FLOWERS,
        .tiles = DecorGfx_PRETTY_FLOWERS,
    },

    [DECOR_COLORFUL_PLANT] =
    {
        .id = DECOR_COLORFUL_PLANT,
        .name = _("COLORFUL PLANT"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_2x2,
        .category = DECORCAT_PLANT,
        .price = 5000,
        .description = DecorDesc_COLORFUL_PLANT,
        .tiles = DecorGfx_COLORFUL_PLANT,
    },

    [DECOR_BIG_PLANT] =
    {
        .id = DECOR_BIG_PLANT,
        .name = _("BIG PLANT"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_2x2,
        .category = DECORCAT_PLANT,
        .price = 5000,
        .description = DecorDesc_BIG_PLANT,
        .tiles = DecorGfx_BIG_PLANT,
    },

    [DECOR_GORGEOUS_PLANT] =
    {
        .id = DECOR_GORGEOUS_PLANT,
        .name = _("GORGEOUS PLANT"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_2x2,
        .category = DECORCAT_PLANT,
        .price = 5000,
        .description = DecorDesc_GORGEOUS_PLANT,
        .tiles = DecorGfx_GORGEOUS_PLANT,
    },

    [DECOR_RED_BRICK] =
    {
        .id = DECOR_RED_BRICK,
        .name = _("RED BRICK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = DecorDesc_RED_BRICK,
        .tiles = DecorGfx_RED_BRICK,
    },

    [DECOR_YELLOW_BRICK] =
    {
        .id = DECOR_YELLOW_BRICK,
        .name = _("YELLOW BRICK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = DecorDesc_YELLOW_BRICK,
        .tiles = DecorGfx_YELLOW_BRICK,
    },

    [DECOR_BLUE_BRICK] =
    {
        .id = DECOR_BLUE_BRICK,
        .name = _("BLUE BRICK"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = DecorDesc_BLUE_BRICK,
        .tiles = DecorGfx_BLUE_BRICK,
    },

    [DECOR_RED_BALLOON] =
    {
        .id = DECOR_RED_BALLOON,
        .name = _("RED BALLOON"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = DecorDesc_RED_BALLOON,
        .tiles = DecorGfx_RED_BALLOON,
    },

    [DECOR_BLUE_BALLOON] =
    {
        .id = DECOR_BLUE_BALLOON,
        .name = _("BLUE BALLOON"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = DecorDesc_BLUE_BALLOON,
        .tiles = DecorGfx_BLUE_BALLOON,
    },

    [DECOR_YELLOW_BALLOON] =
    {
        .id = DECOR_YELLOW_BALLOON,
        .name = _("YELLOW BALLOON"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = DecorDesc_YELLOW_BALLOON,
        .tiles = DecorGfx_YELLOW_BALLOON,
    },

    [DECOR_RED_TENT] =
    {
        .id = DECOR_RED_TENT,
        .name = _("RED TENT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_ORNAMENT,
        .price = 10000,
        .description = DecorDesc_RED_TENT,
        .tiles = DecorGfx_RED_TENT,
    },

    [DECOR_BLUE_TENT] =
    {
        .id = DECOR_BLUE_TENT,
        .name = _("BLUE TENT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_ORNAMENT,
        .price = 10000,
        .description = DecorDesc_BLUE_TENT,
        .tiles = DecorGfx_BLUE_TENT,
    },

    [DECOR_SOLID_BOARD] =
    {
        .id = DECOR_SOLID_BOARD,
        .name = _("SOLID BOARD"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 3000,
        .description = DecorDesc_SOLID_BOARD,
        .tiles = DecorGfx_SOLID_BOARD,
    },

    [DECOR_SLIDE] =
    {
        .id = DECOR_SLIDE,
        .name = _("SLIDE"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_2x4,
        .category = DECORCAT_ORNAMENT,
        .price = 8000,
        .description = DecorDesc_SLIDE,
        .tiles = DecorGfx_SLIDE,
    },

    [DECOR_FENCE_LENGTH] =
    {
        .id = DECOR_FENCE_LENGTH,
        .name = _("FENCE LENGTH"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = DecorDesc_FENCE_LENGTH,
        .tiles = DecorGfx_FENCE_LENGTH,
    },

    [DECOR_FENCE_WIDTH] =
    {
        .id = DECOR_FENCE_WIDTH,
        .name = _("FENCE WIDTH"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 500,
        .description = DecorDesc_FENCE_WIDTH,
        .tiles = DecorGfx_FENCE_WIDTH,
    },

    [DECOR_TIRE] =
    {
        .id = DECOR_TIRE,
        .name = _("TIRE"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_2x2,
        .category = DECORCAT_ORNAMENT,
        .price = 800,
        .description = DecorDesc_TIRE,
        .tiles = DecorGfx_TIRE,
    },

    [DECOR_STAND] =
    {
        .id = DECOR_STAND,
        .name = _("STAND"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_4x2,
        .category = DECORCAT_ORNAMENT,
        .price = 7000,
        .description = DecorDesc_STAND,
        .tiles = DecorGfx_STAND,
    },

    [DECOR_MUD_BALL] =
    {
        .id = DECOR_MUD_BALL,
        .name = _("MUD BALL"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 200,
        .description = DecorDesc_MUD_BALL,
        .tiles = DecorGfx_MUD_BALL,
    },

    [DECOR_BREAKABLE_DOOR] =
    {
        .id = DECOR_BREAKABLE_DOOR,
        .name = _("BREAKABLE DOOR"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 3000,
        .description = DecorDesc_BREAKABLE_DOOR,
        .tiles = DecorGfx_BREAKABLE_DOOR,
    },

    [DECOR_SAND_ORNAMENT] =
    {
        .id = DECOR_SAND_ORNAMENT,
        .name = _("SAND ORNAMENT"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 3000,
        .description = DecorDesc_SAND_ORNAMENT,
        .tiles = DecorGfx_SAND_ORNAMENT,
    },

    [DECOR_SILVER_SHIELD] =
    {
        .id = DECOR_SILVER_SHIELD,
        .name = _("SILVER SHIELD"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 0,
        .description = DecorDesc_SILVER_SHIELD,
        .tiles = DecorGfx_SILVER_SHIELD,
    },

    [DECOR_GOLD_SHIELD] =
    {
        .id = DECOR_GOLD_SHIELD,
        .name = _("GOLD SHIELD"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 0,
        .description = DecorDesc_GOLD_SHIELD,
        .tiles = DecorGfx_GOLD_SHIELD,
    },

    [DECOR_GLASS_ORNAMENT] =
    {
        .id = DECOR_GLASS_ORNAMENT,
        .name = _("GLASS ORNAMENT"),
        .permission = DECORPERM_BEHIND_FLOOR,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_ORNAMENT,
        .price = 0,
        .description = DecorDesc_GLASS_ORNAMENT,
        .tiles = DecorGfx_GLASS_ORNAMENT,
    },

    [DECOR_TV] =
    {
        .id = DECOR_TV,
        .name = _("TV"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 3000,
        .description = DecorDesc_TV,
        .tiles = DecorGfx_TV,
    },

    [DECOR_ROUND_TV] =
    {
        .id = DECOR_ROUND_TV,
        .name = _("ROUND TV"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 4000,
        .description = DecorDesc_ROUND_TV,
        .tiles = DecorGfx_ROUND_TV,
    },

    [DECOR_CUTE_TV] =
    {
        .id = DECOR_CUTE_TV,
        .name = _("CUTE TV"),
        .permission = DECORPERM_SOLID_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_ORNAMENT,
        .price = 4000,
        .description = DecorDesc_CUTE_TV,
        .tiles = DecorGfx_CUTE_TV,
    },

    [DECOR_GLITTER_MAT] =
    {
        .id = DECOR_GLITTER_MAT,
        .name = _("GLITTER MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 2000,
        .description = DecorDesc_GLITTER_MAT,
        .tiles = DecorGfx_GLITTER_MAT,
    },

    [DECOR_JUMP_MAT] =
    {
        .id = DECOR_JUMP_MAT,
        .name = _("JUMP MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 2000,
        .description = DecorDesc_JUMP_MAT,
        .tiles = DecorGfx_JUMP_MAT,
    },

    [DECOR_SPIN_MAT] =
    {
        .id = DECOR_SPIN_MAT,
        .name = _("SPIN MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 2000,
        .description = DecorDesc_SPIN_MAT,
        .tiles = DecorGfx_SPIN_MAT,
    },

    [DECOR_C_LOW_NOTE_MAT] =
    {
        .id = DECOR_C_LOW_NOTE_MAT,
        .name = _("C Low NOTE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = DecorDesc_C_LOW_NOTE_MAT,
        .tiles = DecorGfx_C_LOW_NOTE_MAT,
    },

    [DECOR_D_NOTE_MAT] =
    {
        .id = DECOR_D_NOTE_MAT,
        .name = _("D NOTE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = DecorDesc_D_NOTE_MAT,
        .tiles = DecorGfx_D_NOTE_MAT,
    },

    [DECOR_E_NOTE_MAT] =
    {
        .id = DECOR_E_NOTE_MAT,
        .name = _("E NOTE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = DecorDesc_E_NOTE_MAT,
        .tiles = DecorGfx_E_NOTE_MAT,
    },

    [DECOR_F_NOTE_MAT] =
    {
        .id = DECOR_F_NOTE_MAT,
        .name = _("F NOTE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = DecorDesc_F_NOTE_MAT,
        .tiles = DecorGfx_F_NOTE_MAT,
    },

    [DECOR_G_NOTE_MAT] =
    {
        .id = DECOR_G_NOTE_MAT,
        .name = _("G NOTE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = DecorDesc_G_NOTE_MAT,
        .tiles = DecorGfx_G_NOTE_MAT,
    },

    [DECOR_A_NOTE_MAT] =
    {
        .id = DECOR_A_NOTE_MAT,
        .name = _("A NOTE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = DecorDesc_A_NOTE_MAT,
        .tiles = DecorGfx_A_NOTE_MAT,
    },

    [DECOR_B_NOTE_MAT] =
    {
        .id = DECOR_B_NOTE_MAT,
        .name = _("B NOTE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = DecorDesc_B_NOTE_MAT,
        .tiles = DecorGfx_B_NOTE_MAT,
    },

    [DECOR_C_HIGH_NOTE_MAT] =
    {
        .id = DECOR_C_HIGH_NOTE_MAT,
        .name = _("C High NOTE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_MAT,
        .price = 500,
        .description = DecorDesc_C_HIGH_NOTE_MAT,
        .tiles = DecorGfx_C_HIGH_NOTE_MAT,
    },

    [DECOR_SURF_MAT] =
    {
        .id = DECOR_SURF_MAT,
        .name = _("SURF MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = DecorDesc_SURF_MAT,
        .tiles = DecorGfx_SURF_MAT,
    },

    [DECOR_THUNDER_MAT] =
    {
        .id = DECOR_THUNDER_MAT,
        .name = _("THUNDER MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = DecorDesc_THUNDER_MAT,
        .tiles = DecorGfx_THUNDER_MAT,
    },

    [DECOR_FIRE_BLAST_MAT] =
    {
        .id = DECOR_FIRE_BLAST_MAT,
        .name = _("FIRE BLAST MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = DecorDesc_FIRE_BLAST_MAT,
        .tiles = DecorGfx_FIRE_BLAST_MAT,
    },

    [DECOR_POWDER_SNOW_MAT] =
    {
        .id = DECOR_POWDER_SNOW_MAT,
        .name = _("POWDER SNOW MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = DecorDesc_POWDER_SNOW_MAT,
        .tiles = DecorGfx_POWDER_SNOW_MAT,
    },

    [DECOR_ATTRACT_MAT] =
    {
        .id = DECOR_ATTRACT_MAT,
        .name = _("ATTRACT MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = DecorDesc_ATTRACT_MAT,
        .tiles = DecorGfx_ATTRACT_MAT,
    },

    [DECOR_FISSURE_MAT] =
    {
        .id = DECOR_FISSURE_MAT,
        .name = _("FISSURE MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = DecorDesc_FISSURE_MAT,
        .tiles = DecorGfx_FISSURE_MAT,
    },

    [DECOR_SPIKES_MAT] =
    {
        .id = DECOR_SPIKES_MAT,
        .name = _("SPIKES MAT"),
        .permission = DECORPERM_PASS_FLOOR,
        .shape = DECORSHAPE_3x3,
        .category = DECORCAT_MAT,
        .price = 4000,
        .description = DecorDesc_SPIKES_MAT,
        .tiles = DecorGfx_SPIKES_MAT,
    },

    [DECOR_BALL_POSTER] =
    {
        .id = DECOR_BALL_POSTER,
        .name = _("BALL POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = DecorDesc_BALL_POSTER,
        .tiles = DecorGfx_BALL_POSTER,
    },

    [DECOR_GREEN_POSTER] =
    {
        .id = DECOR_GREEN_POSTER,
        .name = _("GREEN POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = DecorDesc_GREEN_POSTER,
        .tiles = DecorGfx_GREEN_POSTER,
    },

    [DECOR_RED_POSTER] =
    {
        .id = DECOR_RED_POSTER,
        .name = _("RED POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = DecorDesc_RED_POSTER,
        .tiles = DecorGfx_RED_POSTER,
    },

    [DECOR_BLUE_POSTER] =
    {
        .id = DECOR_BLUE_POSTER,
        .name = _("BLUE POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = DecorDesc_BLUE_POSTER,
        .tiles = DecorGfx_BLUE_POSTER,
    },

    [DECOR_CUTE_POSTER] =
    {
        .id = DECOR_CUTE_POSTER,
        .name = _("CUTE POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_POSTER,
        .price = 1000,
        .description = DecorDesc_CUTE_POSTER,
        .tiles = DecorGfx_CUTE_POSTER,
    },

    [DECOR_PIKA_POSTER] =
    {
        .id = DECOR_PIKA_POSTER,
        .name = _("PIKA POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = DecorDesc_PIKA_POSTER,
        .tiles = DecorGfx_PIKA_POSTER,
    },

    [DECOR_LONG_POSTER] =
    {
        .id = DECOR_LONG_POSTER,
        .name = _("LONG POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = DecorDesc_LONG_POSTER,
        .tiles = DecorGfx_LONG_POSTER,
    },

    [DECOR_SEA_POSTER] =
    {
        .id = DECOR_SEA_POSTER,
        .name = _("SEA POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = DecorDesc_SEA_POSTER,
        .tiles = DecorGfx_SEA_POSTER,
    },

    [DECOR_SKY_POSTER] =
    {
        .id = DECOR_SKY_POSTER,
        .name = _("SKY POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = DecorDesc_SKY_POSTER,
        .tiles = DecorGfx_SKY_POSTER,
    },

    [DECOR_KISS_POSTER] =
    {
        .id = DECOR_KISS_POSTER,
        .name = _("KISS POSTER"),
        .permission = DECORPERM_NA_WALL,
        .shape = DECORSHAPE_2x1,
        .category = DECORCAT_POSTER,
        .price = 1500,
        .description = DecorDesc_KISS_POSTER,
        .tiles = DecorGfx_KISS_POSTER,
    },

    [DECOR_PICHU_DOLL] =
    {
        .id = DECOR_PICHU_DOLL,
        .name = _("PICHU DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_PICHU_DOLL,
        .tiles = DecorGfx_PICHU_DOLL,
    },

    [DECOR_PIKACHU_DOLL] =
    {
        .id = DECOR_PIKACHU_DOLL,
        .name = _("PIKACHU DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_PIKACHU_DOLL,
        .tiles = DecorGfx_PIKACHU_DOLL,
    },

    [DECOR_MARILL_DOLL] =
    {
        .id = DECOR_MARILL_DOLL,
        .name = _("MARILL DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_MARILL_DOLL,
        .tiles = DecorGfx_MARILL_DOLL,
    },

    [DECOR_TOGEPI_DOLL] =
    {
        .id = DECOR_TOGEPI_DOLL,
        .name = _("TOGEPI DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_TOGEPI_DOLL,
        .tiles = DecorGfx_TOGEPI_DOLL,
    },

    [DECOR_CYNDAQUIL_DOLL] =
    {
        .id = DECOR_CYNDAQUIL_DOLL,
        .name = _("CYNDAQUIL DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_CYNDAQUIL_DOLL,
        .tiles = DecorGfx_CYNDAQUIL_DOLL,
    },

    [DECOR_CHIKORITA_DOLL] =
    {
        .id = DECOR_CHIKORITA_DOLL,
        .name = _("CHIKORITA DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_CHIKORITA_DOLL,
        .tiles = DecorGfx_CHIKORITA_DOLL,
    },

    [DECOR_TOTODILE_DOLL] =
    {
        .id = DECOR_TOTODILE_DOLL,
        .name = _("TOTODILE DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_TOTODILE_DOLL,
        .tiles = DecorGfx_TOTODILE_DOLL,
    },

    [DECOR_JIGGLYPUFF_DOLL] =
    {
        .id = DECOR_JIGGLYPUFF_DOLL,
        .name = _("JIGGLYPUFF DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_JIGGLYPUFF_DOLL,
        .tiles = DecorGfx_JIGGLYPUFF_DOLL,
    },

    [DECOR_MEOWTH_DOLL] =
    {
        .id = DECOR_MEOWTH_DOLL,
        .name = _("MEOWTH DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_MEOWTH_DOLL,
        .tiles = DecorGfx_MEOWTH_DOLL,
    },

    [DECOR_CLEFAIRY_DOLL] =
    {
        .id = DECOR_CLEFAIRY_DOLL,
        .name = _("CLEFAIRY DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_CLEFAIRY_DOLL,
        .tiles = DecorGfx_CLEFAIRY_DOLL,
    },

    [DECOR_DITTO_DOLL] =
    {
        .id = DECOR_DITTO_DOLL,
        .name = _("DITTO DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_DITTO_DOLL,
        .tiles = DecorGfx_DITTO_DOLL,
    },

    [DECOR_SMOOCHUM_DOLL] =
    {
        .id = DECOR_SMOOCHUM_DOLL,
        .name = _("SMOOCHUM DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_SMOOCHUM_DOLL,
        .tiles = DecorGfx_SMOOCHUM_DOLL,
    },

    [DECOR_TREECKO_DOLL] =
    {
        .id = DECOR_TREECKO_DOLL,
        .name = _("TREECKO DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_TREECKO_DOLL,
        .tiles = DecorGfx_TREECKO_DOLL,
    },

    [DECOR_TORCHIC_DOLL] =
    {
        .id = DECOR_TORCHIC_DOLL,
        .name = _("TORCHIC DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_TORCHIC_DOLL,
        .tiles = DecorGfx_TORCHIC_DOLL,
    },

    [DECOR_MUDKIP_DOLL] =
    {
        .id = DECOR_MUDKIP_DOLL,
        .name = _("MUDKIP DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_MUDKIP_DOLL,
        .tiles = DecorGfx_MUDKIP_DOLL,
    },

    [DECOR_DUSKULL_DOLL] =
    {
        .id = DECOR_DUSKULL_DOLL,
        .name = _("DUSKULL DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_DUSKULL_DOLL,
        .tiles = DecorGfx_DUSKULL_DOLL,
    },

    [DECOR_WYNAUT_DOLL] =
    {
        .id = DECOR_WYNAUT_DOLL,
        .name = _("WYNAUT DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_WYNAUT_DOLL,
        .tiles = DecorGfx_WYNAUT_DOLL,
    },

    [DECOR_BALTOY_DOLL] =
    {
        .id = DECOR_BALTOY_DOLL,
        .name = _("BALTOY DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_BALTOY_DOLL,
        .tiles = DecorGfx_BALTOY_DOLL,
    },

    [DECOR_KECLEON_DOLL] =
    {
        .id = DECOR_KECLEON_DOLL,
        .name = _("KECLEON DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_KECLEON_DOLL,
        .tiles = DecorGfx_KECLEON_DOLL,
    },

    [DECOR_AZURILL_DOLL] =
    {
        .id = DECOR_AZURILL_DOLL,
        .name = _("AZURILL DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_AZURILL_DOLL,
        .tiles = DecorGfx_AZURILL_DOLL,
    },

    [DECOR_SKITTY_DOLL] =
    {
        .id = DECOR_SKITTY_DOLL,
        .name = _("SKITTY DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_SKITTY_DOLL,
        .tiles = DecorGfx_SKITTY_DOLL,
    },

    [DECOR_SWABLU_DOLL] =
    {
        .id = DECOR_SWABLU_DOLL,
        .name = _("SWABLU DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_SWABLU_DOLL,
        .tiles = DecorGfx_SWABLU_DOLL,
    },

    [DECOR_GULPIN_DOLL] =
    {
        .id = DECOR_GULPIN_DOLL,
        .name = _("GULPIN DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_GULPIN_DOLL,
        .tiles = DecorGfx_GULPIN_DOLL,
    },

    [DECOR_LOTAD_DOLL] =
    {
        .id = DECOR_LOTAD_DOLL,
        .name = _("LOTAD DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_LOTAD_DOLL,
        .tiles = DecorGfx_LOTAD_DOLL,
    },

    [DECOR_SEEDOT_DOLL] =
    {
        .id = DECOR_SEEDOT_DOLL,
        .name = _("SEEDOT DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_DOLL,
        .price = 3000,
        .description = DecorDesc_SEEDOT_DOLL,
        .tiles = DecorGfx_SEEDOT_DOLL,
    },

    [DECOR_PIKA_CUSHION] =
    {
        .id = DECOR_PIKA_CUSHION,
        .name = _("PIKA CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_PIKA_CUSHION,
        .tiles = DecorGfx_PIKA_CUSHION,
    },

    [DECOR_ROUND_CUSHION] =
    {
        .id = DECOR_ROUND_CUSHION,
        .name = _("ROUND CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_ROUND_CUSHION,
        .tiles = DecorGfx_ROUND_CUSHION,
    },

    [DECOR_KISS_CUSHION] =
    {
        .id = DECOR_KISS_CUSHION,
        .name = _("KISS CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_KISS_CUSHION,
        .tiles = DecorGfx_KISS_CUSHION,
    },

    [DECOR_ZIGZAG_CUSHION] =
    {
        .id = DECOR_ZIGZAG_CUSHION,
        .name = _("ZIGZAG CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_ZIGZAG_CUSHION,
        .tiles = DecorGfx_ZIGZAG_CUSHION,
    },

    [DECOR_SPIN_CUSHION] =
    {
        .id = DECOR_SPIN_CUSHION,
        .name = _("SPIN CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_SPIN_CUSHION,
        .tiles = DecorGfx_SPIN_CUSHION,
    },

    [DECOR_DIAMOND_CUSHION] =
    {
        .id = DECOR_DIAMOND_CUSHION,
        .name = _("DIAMOND CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_DIAMOND_CUSHION,
        .tiles = DecorGfx_DIAMOND_CUSHION,
    },

    [DECOR_BALL_CUSHION] =
    {
        .id = DECOR_BALL_CUSHION,
        .name = _("BALL CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_BALL_CUSHION,
        .tiles = DecorGfx_BALL_CUSHION,
    },

    [DECOR_GRASS_CUSHION] =
    {
        .id = DECOR_GRASS_CUSHION,
        .name = _("GRASS CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_GRASS_CUSHION,
        .tiles = DecorGfx_GRASS_CUSHION,
    },

    [DECOR_FIRE_CUSHION] =
    {
        .id = DECOR_FIRE_CUSHION,
        .name = _("FIRE CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_FIRE_CUSHION,
        .tiles = DecorGfx_FIRE_CUSHION,
    },

    [DECOR_WATER_CUSHION] =
    {
        .id = DECOR_WATER_CUSHION,
        .name = _("WATER CUSHION"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x1,
        .category = DECORCAT_CUSHION,
        .price = 2000,
        .description = DecorDesc_WATER_CUSHION,
        .tiles = DecorGfx_WATER_CUSHION,
    },

    [DECOR_SNORLAX_DOLL] =
    {
        .id = DECOR_SNORLAX_DOLL,
        .name = _("SNORLAX DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_SNORLAX_DOLL,
        .tiles = DecorGfx_SNORLAX_DOLL,
    },

    [DECOR_RHYDON_DOLL] =
    {
        .id = DECOR_RHYDON_DOLL,
        .name = _("RHYDON DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_RHYDON_DOLL,
        .tiles = DecorGfx_RHYDON_DOLL,
    },

    [DECOR_LAPRAS_DOLL] =
    {
        .id = DECOR_LAPRAS_DOLL,
        .name = _("LAPRAS DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_LAPRAS_DOLL,
        .tiles = DecorGfx_LAPRAS_DOLL,
    },

    [DECOR_VENUSAUR_DOLL] =
    {
        .id = DECOR_VENUSAUR_DOLL,
        .name = _("VENUSAUR DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_VENUSAUR_DOLL,
        .tiles = DecorGfx_VENUSAUR_DOLL,
    },

    [DECOR_CHARIZARD_DOLL] =
    {
        .id = DECOR_CHARIZARD_DOLL,
        .name = _("CHARIZARD DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_CHARIZARD_DOLL,
        .tiles = DecorGfx_CHARIZARD_DOLL,
    },

    [DECOR_BLASTOISE_DOLL] =
    {
        .id = DECOR_BLASTOISE_DOLL,
        .name = _("BLASTOISE DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_BLASTOISE_DOLL,
        .tiles = DecorGfx_BLASTOISE_DOLL,
    },

    [DECOR_WAILMER_DOLL] =
    {
        .id = DECOR_WAILMER_DOLL,
        .name = _("WAILMER DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_WAILMER_DOLL,
        .tiles = DecorGfx_WAILMER_DOLL,
    },

    [DECOR_REGIROCK_DOLL] =
    {
        .id = DECOR_REGIROCK_DOLL,
        .name = _("REGIROCK DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_REGIROCK_DOLL,
        .tiles = DecorGfx_REGIROCK_DOLL,
    },

    [DECOR_REGICE_DOLL] =
    {
        .id = DECOR_REGICE_DOLL,
        .name = _("REGICE DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_REGICE_DOLL,
        .tiles = DecorGfx_REGICE_DOLL,
    },

    [DECOR_REGISTEEL_DOLL] =
    {
        .id = DECOR_REGISTEEL_DOLL,
        .name = _("REGISTEEL DOLL"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_DOLL,
        .price = 10000,
        .description = DecorDesc_REGISTEEL_DOLL,
        .tiles = DecorGfx_REGISTEEL_DOLL,
    }/*,
    
    [DECOR_CHERI_FLOWER] =
    {
        .id = DECOR_CHERI_FLOWER,
        .name = _("CHERI FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25, // MOD CONTEST How to modify this to work like on DP? ALSO how to hide it until berry is first Acquired
        .description = DecorDesc_CHERI_FLOWER,
        .tiles = DecorGfx_CHERI_FLOWER,
    },

    [DECOR_CHESTO_FLOWER] =
    {
        .id = DECOR_CHESTO_FLOWER,
        .name = _("CHESTO FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_CHESTO_FLOWER,
        .tiles = DecorGfx_CHESTO_FLOWER,
    },

    [DECOR_PECHA_FLOWER] =
    {
        .id = DECOR_PECHA_FLOWER,
        .name = _("PECHA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_PECHA_FLOWER,
        .tiles = DecorGfx_PECHA_FLOWER,
    },

    [DECOR_RAWST_FLOWER] =
    {
        .id = DECOR_RAWST_FLOWER,
        .name = _("RAWST FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_RAWST_FLOWER,
        .tiles = DecorGfx_RAWST_FLOWER,
    },

    [DECOR_ASPEAR_FLOWER] =
    {
        .id = DECOR_ASPEAR_FLOWER,
        .name = _("ASPEAR FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_ASPEAR_FLOWER,
        .tiles = DecorGfx_ASPEAR_FLOWER,
    },

    [DECOR_LEPPA_FLOWER] =
    {
        .id = DECOR_LEPPA_FLOWER,
        .name = _("LEPPA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_LEPPA_FLOWER,
        .tiles = DecorGfx_LEPPA_FLOWER,
    },

    [DECOR_ORAN_FLOWER] =
    {
        .id = DECOR_ORAN_FLOWER,
        .name = _("ORAN FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_ORAN_FLOWER,
        .tiles = DecorGfx_ORAN_FLOWER,
    },

    [DECOR_PERSIM_FLOWER] =
    {
        .id = DECOR_PERSIM_FLOWER,
        .name = _("PERSIM FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_PERSIM_FLOWER,
        .tiles = DecorGfx_PERSIM_FLOWER,
    },

    [DECOR_LUM_FLOWER] =
    {
        .id = DECOR_LUM_FLOWER,
        .name = _("LUM FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_LUM_FLOWER,
        .tiles = DecorGfx_LUM_FLOWER,
    },

    [DECOR_SITRUS_FLOWER] =
    {
        .id = DECOR_SITRUS_FLOWER,
        .name = _("SITRUS FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_SITRUS_FLOWER,
        .tiles = DecorGfx_SITRUS_FLOWER,
    },

    [DECOR_FIGY_FLOWER] =
    {
        .id = DECOR_FIGY_FLOWER,
        .name = _("FIGY FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_FIGY_FLOWER,
        .tiles = DecorGfx_FIGY_FLOWER,
    },

    [DECOR_WIKI_FLOWER] =
    {
        .id = DECOR_WIKI_FLOWER,
        .name = _("WIKI FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_WIKI_FLOWER,
        .tiles = DecorGfx_WIKI_FLOWER,
    },

    [DECOR_MAGO_FLOWER] =
    {
        .id = DECOR_MAGO_FLOWER,
        .name = _("MAGO FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_MAGO_FLOWER,
        .tiles = DecorGfx_MAGO_FLOWER,
    },

    [DECOR_AGUAV_FLOWER] =
    {
        .id = DECOR_AGUAV_FLOWER,
        .name = _("AGUAV FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_AGUAV_FLOWER,
        .tiles = DecorGfx_AGUAV_FLOWER,
    },

    [DECOR_IAPAPA_FLOWER] =
    {
        .id = DECOR_IAPAPA_FLOWER,
        .name = _("IAPAPA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_IAPAPA_FLOWER,
        .tiles = DecorGfx_IAPAPA_FLOWER,
    },

    [DECOR_RAZZ_FLOWER] =
    {
        .id = DECOR_RAZZ_FLOWER,
        .name = _("RAZZ FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 50,
        .description = DecorDesc_RAZZ_FLOWER,
        .tiles = DecorGfx_RAZZ_FLOWER,
    },

    [DECOR_BLUK_FLOWER] =
    {
        .id = DECOR_BLUK_FLOWER,
        .name = _("BLUK FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 50,
        .description = DecorDesc_BLUK_FLOWER,
        .tiles = DecorGfx_BLUK_FLOWER,
    },

    [DECOR_NANAB_FLOWER] =
    {
        .id = DECOR_NANAB_FLOWER,
        .name = _("NANAB FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 50,
        .description = DecorDesc_NANAB_FLOWER,
        .tiles = DecorGfx_NANAB_FLOWER,
    },

    [DECOR_WEPEAR_FLOWER] =
    {
        .id = DECOR_WEPEAR_FLOWER,
        .name = _("WEPEAR FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_WEPEAR_FLOWER,
        .tiles = DecorGfx_WEPEAR_FLOWER,
    },

    [DECOR_PINAP_FLOWER] =
    {
        .id = DECOR_PINAP_FLOWER,
        .name = _("PINAP FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_PINAP_FLOWER,
        .tiles = DecorGfx_PINAP_FLOWER,
    },

    [DECOR_POMEG_FLOWER] =
    {
        .id = DECOR_POMEG_FLOWER,
        .name = _("POMEG FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_POMEG_FLOWER,
        .tiles = DecorGfx_POMEG_FLOWER,
    },

    [DECOR_KELPSY_FLOWER] =
    {
        .id = DECOR_KELPSY_FLOWER,
        .name = _("KELPSY FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_KELPSY_FLOWER,
        .tiles = DecorGfx_KELPSY_FLOWER,
    },

    [DECOR_QUALOT_FLOWER] =
    {
        .id = DECOR_QUALOT_FLOWER,
        .name = _("QUALOT FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_QUALOT_FLOWER,
        .tiles = DecorGfx_QUALOT_FLOWER,
    },

    [DECOR_HONDEW_FLOWER] =
    {
        .id = DECOR_HONDEW_FLOWER,
        .name = _("HONDEW FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_HONDEW_FLOWER,
        .tiles = DecorGfx_HONDEW_FLOWER,
    },

    [DECOR_GREPA_FLOWER] =
    {
        .id = DECOR_GREPA_FLOWER,
        .name = _("GREPA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_GREPA_FLOWER,
        .tiles = DecorGfx_GREPA_FLOWER,
    },

    [DECOR_TAMATO_FLOWER] =
    {
        .id = DECOR_TAMATO_FLOWER,
        .name = _("TAMATO FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_TAMATO_FLOWER,
        .tiles = DecorGfx_TAMATO_FLOWER,
    },

    [DECOR_CORNN_FLOWER] =
    {
        .id = DECOR_CORNN_FLOWER,
        .name = _("CORNN FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 50,
        .description = DecorDesc_CORNN_FLOWER,
        .tiles = DecorGfx_CORNN_FLOWER,
    },

    [DECOR_MAGOST_FLOWER] =
    {
        .id = DECOR_MAGOST_FLOWER,
        .name = _("MAGOST FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 50,
        .description = DecorDesc_MAGOST_FLOWER,
        .tiles = DecorGfx_MAGOST_FLOWER,
    },

    [DECOR_RABUTA_FLOWER] =
    {
        .id = DECOR_RABUTA_FLOWER,
        .name = _("RABUTA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 20,
        .description = DecorDesc_RABUTA_FLOWER,
        .tiles = DecorGfx_RABUTA_FLOWER,
    },

    [DECOR_NOMEL_FLOWER] =
    {
        .id = DECOR_NOMEL_FLOWER,
        .name = _("NOMEL FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 50,
        .description = DecorDesc_NOMEL_FLOWER,
        .tiles = DecorGfx_NOMEL_FLOWER,
    },

    [DECOR_SPELON_FLOWER] =
    {
        .id = DECOR_SPELON_FLOWER,
        .name = _("SPELON FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 75,
        .description = DecorDesc_SPELON_FLOWER,
        .tiles = DecorGfx_SPELON_FLOWER,
    },

    [DECOR_PAMTRE_FLOWER] =
    {
        .id = DECOR_PAMTRE_FLOWER,
        .name = _("PAMTRE FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 75,
        .description = DecorDesc_PAMTRE_FLOWER,
        .tiles = DecorGfx_PAMTRE_FLOWER,
    },

    [DECOR_WATMEL_FLOWER] =
    {
        .id = DECOR_WATMEL_FLOWER,
        .name = _("WATMEL FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 75,
        .description = DecorDesc_WATMEL_FLOWER,
        .tiles = DecorGfx_WATMEL_FLOWER,
    },

    [DECOR_DURIN_FLOWER] =
    {
        .id = DECOR_DURIN_FLOWER,
        .name = _("DURIN FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 75,
        .description = DecorDesc_DURIN_FLOWER,
        .tiles = DecorGfx_DURIN_FLOWER,
    },

    [DECOR_BELUE_FLOWER] =
    {
        .id = DECOR_BELUE_FLOWER,
        .name = _("BELUE FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 75,
        .description = DecorDesc_BELUE_FLOWER,
        .tiles = DecorGfx_BELUE_FLOWER,
    },

    [DECOR_PAPRIK_FLOWER] =
    {
        .id = DECOR_PAPRIK_FLOWER,
        .name = _("PAPRIK FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_PAPRIK_FLOWER,
        .tiles = DecorGfx_PAPRIK_FLOWER,
    },

    [DECOR_EGGANT_FLOWER] =
    {
        .id = DECOR_EGGANT_FLOWER,
        .name = _("EGGANT FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_EGGANT_FLOWER,
        .tiles = DecorGfx_EGGANT_FLOWER,
    },

    [DECOR_DRASH_FLOWER] =
    {
        .id = DECOR_DRASH_FLOWER,
        .name = _("DRASH FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_DRASH_FLOWER,
        .tiles = DecorGfx_DRASH_FLOWER,
    },

    [DECOR_SKUASH_FLOWER] =
    {
        .id = DECOR_SKUASH_FLOWER,
        .name = _("SKUASH FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 30,
        .description = DecorDesc_RSKUASH_FLOWER,
        .tiles = DecorGfx_SKUASH_FLOWER,
    },

    [DECOR_PUMKIN_FLOWER] =
    {
        .id = DECOR_PUMKIN_FLOWER,
        .name = _("PUMKIN FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 39,
        .description = DecorDesc_PUMKIN_FLOWER,
        .tiles = DecorGfx_PUMKIN_FLOWER,
    },

    [DECOR_CHILAN_FLOWER] =
    {
        .id = DECOR_CHILAN_FLOWER,
        .name = _("CHILAN FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 20,
        .description = DecorDesc_CHILAN_FLOWER,
        .tiles = DecorGfx_CHILAN_FLOWER,
    },

    [DECOR_MALIUM_FLOWER] =
    {
        .id = DECOR_MALIUM_FLOWER,
        .name = _("MALIUM FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 29,
        .description = DecorDesc_MALIUM_FLOWER,
        .tiles = DecorGfx_MALIUM_FLOWER,
    },

    [DECOR_TOPO_FLOWER] =
    {
        .id = DECOR_TOPO_FLOWER,
        .name = _("TOPO FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 100,
        .description = DecorDesc_TOPO_FLOWER,
        .tiles = DecorGfx_TOPO_FLOWER,
    },

    [DECOR_STRIB_FLOWER] =
    {
        .id = DECOR_STRIB_FLOWER,
        .name = _("STRIB FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 100,
        .description = DecorDesc_STRIB_FLOWER,
        .tiles = DecorGfx_STRIB_FLOWER,
    },

    [DECOR_NOION_FLOWER] =
    {
        .id = DECOR_NOION_FLOWER,
        .name = _("NOION FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 16,
        .description = DecorDesc_NOION_FLOWER,
        .tiles = DecorGfx_NOION_FLOWER,
    },

    [DECOR_NUTPEA_FLOWER] =
    {
        .id = DECOR_NUTPEA_FLOWER,
        .name = _("NUTPEA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_NUTPEA_FLOWER,
        .tiles = DecorGfx_NUTPEA_FLOWER,
    },

    [DECOR_ROKA_FLOWER] =
    {
        .id = DECOR_ROKA_FLOWER,
        .name = _("ROKA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_ROKA_FLOWER,
        .tiles = DecorGfx_ROKA_FLOWER,
    },

    [DECOR_LIECHI_FLOWER] =
    {
        .id = DECOR_LIECHI_FLOWER,
        .name = _("LIECHI FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_LIECHI_FLOWER,
        .tiles = DecorGfx_LIECHI_FLOWER,
    },

    [DECOR_GANLON_FLOWER] =
    {
        .id = DECOR_GANLON_FLOWER,
        .name = _("GANLON FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_GANLON_FLOWER,
        .tiles = DecorGfx_GANLON_FLOWER,
    },

    [DECOR_SALAC_FLOWER] =
    {
        .id = DECOR_SALAC_FLOWER,
        .name = _("SALAC FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_SALAC_FLOWER,
        .tiles = DecorGfx_SALAC_FLOWER,
    },

    [DECOR_PETAYA_FLOWER] =
    {
        .id = DECOR_PETAYA_FLOWER,
        .name = _("PETAYA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_PETAYA_FLOWER,
        .tiles = DecorGfx_PETAYA_FLOWER,
    },

    [DECOR_APICOT_FLOWER] =
    {
        .id = DECOR_APICOT_FLOWER,
        .name = _("APICOT FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 25,
        .description = DecorDesc_APICOT_FLOWER,
        .tiles = DecorGfx_APICOT_FLOWER,
    },

    [DECOR_LANSAT_FLOWER] =
    {
        .id = DECOR_LANSAT_FLOWER,
        .name = _("LANSAT FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 7,
        .description = DecorDesc_LANSAT_FLOWER,
        .tiles = DecorGfx_LANSAT_FLOWER,
    },
    
    [DECOR_STARF_FLOWER] =
    {
        .id = DECOR_STARF_FLOWER,
        .name = _("STARF FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 7,
        .description = DecorDesc_STARF_FLOWER,
        .tiles = DecorGfx_STARF_FLOWER,
    },

    [DECOR_ENIGMA_FLOWER] =
    {
        .id = DECOR_ENIGMA_FLOWER,
        .name = _("ENIGMA FLOWER"),
        .permission = DECORPERM_SPRITE,
        .shape = DECORSHAPE_1x2,
        .category = DECORCAT_PLANT,
        .price = 7,
        .description = DecorDesc_ENIGMA_FLOWER,
        .tiles = DecorGfx_ENIGMA_FLOWER,
    }*/
};
