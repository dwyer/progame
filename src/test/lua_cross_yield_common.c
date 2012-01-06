void
resume_coroutine (lua_State *thr) {
	 int err;

#ifdef LUA51
	 err = lua_resume (thr, 0);
#endif
#ifdef LUA52
	 err = lua_resume (thr, NULL, 0);
#endif

	 if (LUA_OK != err && LUA_YIELD != err) {
		  printf ("ERROR WHILE RESUMING: %s\n", lua_tostring (thr, -1));
		  assert (0);
	 }

	 lua_settop (thr, 0);
}

int return_normally_cfunc (lua_State *L) {
	 int ret, ctx;

	 printf ("Continuation\n");

	 return 0;
}

void return_normally (lua_State *L) {
	 lua_State *thr;

	 thr = lua_newthread (L);

	 lua_pushcfunction (L, return_normally_cfunc);
	 lua_xmove (L, thr, 1);

	 resume_coroutine (thr);
}

void helper_push_yield_lua_func (lua_State *L) {
	 const char *fstr = "coroutine.yield (); return 0;";
	 int err;

	 err = luaL_loadstring (L, fstr);

	 if (LUA_OK != err)
	 {
		  printf ("ERR LOADING CHUNK: %s\n", lua_tostring (L, -1));
		  assert (0);
	 }
}

int return_using_yield_cfunc (lua_State *L) {
	 int ret, ctx;

	 printf ("Continuation\n");

	 helper_push_yield_lua_func (L);

#ifdef LUA51
	 lua_call (L, 0, 0);
#endif
#ifdef LUA52
	 lua_callk (L, 0, 0, 0, return_using_yield_cfunc);
#endif

	 return 0;
}

void return_using_yield (lua_State *L) {
	 lua_State *thr;

	 thr = lua_newthread (L);

	 lua_pushcfunction (L, return_using_yield_cfunc);
	 lua_xmove (L, thr, 1);

	 resume_coroutine (thr);
}
