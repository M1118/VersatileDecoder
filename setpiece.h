class SetPiece {
	public:
		SetPiece();
		bool isActive() { return active; };
	private:
		bool	active;
};

extern SetPiece setPiece;
